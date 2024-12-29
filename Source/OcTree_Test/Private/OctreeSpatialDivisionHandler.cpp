// Fill out your copyright notice in the Description page of Project Settings.


#include "OctreeSpatialDivisionHandler.h"

#include <bitset>

#include "Octree.h"
#include "OctreeNode.h"
#include "Engine/OverlapResult.h"
#include "Kismet/KismetSystemLibrary.h"

UOctreeSpatialDivisionHandler::UOctreeSpatialDivisionHandler()
{
}

UOctreeSpatialDivisionHandler::~UOctreeSpatialDivisionHandler()
{
	_octree->DeleteNode();
}

void UOctreeSpatialDivisionHandler::CreateOctreeSpatial(FVector size, FVector origin, int32 finishDepth)
{
	_octree = NewObject<UOctree>(this);
	_octree->CreateNode(size, origin, finishDepth);
}

void UOctreeSpatialDivisionHandler::CreateStaticMeshMap(const UWorld* world)
{
	TArray<FOverlapResult> results;
	FCollisionObjectQueryParams queryParams;
	queryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	
	bool bOverlap = world->OverlapMultiByObjectType(
		results,
		_octree->GetOrigin(),
		FQuat::Identity,
		queryParams,
		FCollisionShape::MakeBox(_octree->GetSize())
	);

	// デバッグ用
	// UKismetSystemLibrary::DrawDebugBox(world, _octree->GetOrigin(), _octree->GetSize(), FColor::Orange, FRotator::ZeroRotator, 1000);

	if (bOverlap)
	{
		TArray<AActor*> actors;
		for (const auto& result : results)
		{
			actors.Add(result.GetActor());
		}

		StaticMeshMapping(actors);
	}
}

UOctreeNode* UOctreeSpatialDivisionHandler::FindNode(const FVector& targetLocation) const 
{
	return _octree->FindNode(targetLocation);
}

UOctreeNode* UOctreeSpatialDivisionHandler::FindNode(const FVector& leftTopFront, const FVector& rightBottomBack) const 
{
	return _octree->FindNode(leftTopFront, rightBottomBack);
}

void UOctreeSpatialDivisionHandler::ActiveStaticMeshToLocation(const FVector& leftTopFront, const FVector& rightBottomBack)
{
	if (_staticMeshMap.IsEmpty()){ return; }

	AllStaticMeshUnActive();

	const auto node = FindNode(leftTopFront, rightBottomBack);
	if (!node){ return; }
	
	const auto id = node->GetId();
	const auto depth = node->GetDepth() + 1; // RootNode + 層

	// 該当NodeIDとその親NodeIDをKeyにするメッシュのVisibilityを有効にする
	auto tempId = id;
	for (size_t i = 0; i < depth; ++i)
	{
		const size_t sliceBit = 3 * (depth - i);
		const auto idBitString = FString(std::bitset<64>(tempId).to_string().substr(64 - sliceBit).c_str());

		if (_staticMeshMap.Contains(idBitString))
		{
			for (const auto& mesh : _staticMeshMap[idBitString])
			{
				mesh->SetVisibility(true);
			}
		}

		// 次の親ノードのIDにシフト（親ノードへの移動）
		tempId >>= 3;
	}
}

void UOctreeSpatialDivisionHandler::StaticMeshMapping(TArray<AActor*> actors)
{
	for (const AActor* actor : actors)
	{
		// アクターが StaticMeshComponent を持つか確認
		UStaticMeshComponent* staticMeshComp = Cast<UStaticMeshComponent>(
			actor->GetComponentByClass(UStaticMeshComponent::StaticClass())
		);

		if (staticMeshComp)
		{
			FBoxSphereBounds bounds = staticMeshComp->Bounds;
			FVector center = bounds.Origin;

			// サイズ（ワールドサイズを計算）
			FVector meshSize = bounds.BoxExtent;

			// デバッグ用
			UKismetSystemLibrary::DrawDebugBox(actor, center, meshSize, FColor::Purple, FRotator::ZeroRotator, 1000);

			// オクトツリーの該当IDに所属させる
			const auto leftTopFront = (center + meshSize * 0.5f) ;
			const auto rightButtonBack = (center - meshSize * 0.5f);

			if (auto node = _octree->FindNode(leftTopFront, rightButtonBack))
			{
				const size_t sliceBit = 3 * (node->GetDepth() + 1);
				auto nodeIdBitString = FString(std::bitset<64>(node->GetId()).to_string().substr(64 - sliceBit).c_str());

				if (!_staticMeshMap.Contains(nodeIdBitString))
				{
					_staticMeshMap.Add(nodeIdBitString);
				}

				_staticMeshMap[nodeIdBitString].Add(staticMeshComp);
			}
		}
	}
}

void UOctreeSpatialDivisionHandler::AllStaticMeshUnActive()
{
	for (const auto& pair : _staticMeshMap)
	{
		for (const auto& mesh : pair.Value)
		{
			mesh->SetVisibility(false);
		}
	}
}
