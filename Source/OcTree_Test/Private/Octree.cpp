// Fill out your copyright notice in the Description page of Project Settings.


#include "Octree.h"

#include <bitset>

#include "MortonFunctionLibrary.h"
#include "OctreeNode.h"

UOctree::UOctree()
	: _rootNode(nullptr), _organizeMask(0), _maxDepth(0)
{
}

void UOctree::CreateNode(const FVector& size, const FVector& origin, const uint32 finishDepth)
{
	_maxDepth = finishDepth;
	_organizeMask = ((1ULL << (_maxDepth * 3)) - 1);
	_origin = origin;
	_size = size;
	_unitVec = _size / (1 << (_maxDepth - 1));

	_rootNode = NewObject<UOctreeNode>(this);
	_rootNode->AddToRoot();
	_rootNode->SetMaxDepth(_maxDepth);
	_rootNode->CreateChildren(_size, _origin);
}

UOctreeNode* UOctree::FindNode(const FVector& location) const
{
	const auto unitLocation = (location - _origin + _size) / _unitVec;
	const auto locationMortonCode = UMortonFunctionLibrary::EncodeMorton3D(unitLocation) & _organizeMask;
	// デバッグ用
	// auto locationMortonBit = std::bitset<32>(locationMortonCode).to_string();
	
	return _rootNode->FindNode(locationMortonCode);
}

UOctreeNode* UOctree::FindNode(const FVector& leftTopFront, const FVector& rightBottomBack)
{
	const auto size =  _rootNode->GetSize();
	
	const auto unitLeftTopFront = (leftTopFront - _origin + size) / _unitVec;
	const auto unitRightButtonBack = (rightBottomBack - _origin + size) / _unitVec;

	const auto leftTopFrontMortonCode = UMortonFunctionLibrary::EncodeMorton3D(unitLeftTopFront);
	const auto rightBottomBackMortonCode = UMortonFunctionLibrary::EncodeMorton3D(unitRightButtonBack);

	// 必要なBit以外を0にする
	auto xorCode = (leftTopFrontMortonCode ^ rightBottomBackMortonCode) & _organizeMask;
	// デバッグ用
	// auto xorCodeBit = std::bitset<32>(xorCode).to_string();
	
	uint32 depth = 0;
	while (_maxDepth > depth)
	{
		const auto checkMask = ((1ULL << ((depth) * 3)) - 1) << ((_maxDepth - depth) * 3);
		if ((xorCode & checkMask) != 0)
		{
			break;
		}
		
		depth++;

		// デバッグ用
		// auto checkBit = std::bitset<32>(xorCode & checkMask).to_string();
		// auto checkMaskBit = std::bitset<32>(checkMask).to_string();
		// auto shiftCodeBit = std::bitset<32>(xorCode).to_string();
	}

	return _rootNode->FindNode(rightBottomBackMortonCode, depth);
}

void UOctree::DeleteNode()
{
	if (!_rootNode) { return; }
	_rootNode->RemoveFromRoot();
	_rootNode = nullptr;
}
