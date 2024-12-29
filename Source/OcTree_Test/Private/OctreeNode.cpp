// Fill out your copyright notice in the Description page of Project Settings.


#include "OctreeNode.h"

#include <bitset>

uint16 UOctreeNode::MaxDepth = 2;

UOctreeNode::UOctreeNode(): _depth(0), _id(0), _mask(0)
{
}

UOctreeNode::~UOctreeNode()
{
	_children.Empty();
}

// 8つの子ノードを作成
void UOctreeNode::CreateChildren(const FVector& size, const FVector& origin, UOctreeNode* parent, const uint32 depth, const uint64 id)
{
	_parent = parent;
	_size = size;
	_origin = origin;
	_depth = depth;
	_id = id;

	const uint32 childDepth = depth + 1;
	if (childDepth > MaxDepth)
	{
		return;
	}

	const FVector childSize = _size / 2.0f;
	const auto childBitMask = (1ULL << ((childDepth + 1) * 3)) - 1;
	// デバッグ用
	// auto maskBit = std::bitset<32>(mask).to_string();
	for (size_t i = 0; i < ChildCount; ++i)
	{
		const FVector offset(
			(i & 1 ? 1 : -1) * childSize.X,
			(i & 2 ? 1 : -1) * childSize.Y,
			(i & 4 ? 1 : -1) * childSize.Z
		);
		
		const FVector childOrigin = _origin + offset;
		const auto childId = (_id << 3) + i;
		auto child = NewObject<UOctreeNode>(this);
		child->SetMask(childBitMask);
		child->CreateChildren(childSize, childOrigin, this, childDepth, childId);
		_children.Add(child);
	}
}

UOctreeNode* UOctreeNode::FindNode(uint64 mortonCode, const uint32 finishDepth)
{
	if (_depth >= finishDepth)
	{
		return nullptr;
	}
	
	const auto checkMortonCode = (mortonCode >> ((MaxDepth - _depth) * 3));
	// デバッグ用
	// auto checkMortonBit = std::bitset<32>(checkMortonCode).to_string();
	
	if (MortonCodeCheck(checkMortonCode))
	{
		// 子ノードを再帰的に探索
		for (auto child : _children)
		{
			if (const auto childResult = child->FindNode(mortonCode, finishDepth))
			{
				return childResult; // 見つかったノードを返す
			}
		}
		
		return this; // 見つからなかった場合は自身を返す
	}

	return nullptr;
}

bool UOctreeNode::MortonCodeCheck(const uint64 mortonCode) const
{
	// デバッグ用
	// auto maskBit = std::bitset<32>(_mask).to_string();
	// auto locationBit = std::bitset<32>(mortonCode).to_string();
	// auto nodeIdBit = std::bitset<32>(_id).to_string();
	
	if ((mortonCode & _mask) == (_id & _mask))
	{
		return true;
	}
	return false;
}
