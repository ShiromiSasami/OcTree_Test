// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OctreeSpatialDivisionHandler.generated.h"

class UOctree;
class UOctreeNode;

/**
 * 
 */
UCLASS()
class OCTREE_TEST_API UOctreeSpatialDivisionHandler : public UObject
{
	GENERATED_BODY()

public:
	UOctreeSpatialDivisionHandler();
	virtual ~UOctreeSpatialDivisionHandler() override;
	

	UFUNCTION(BlueprintCallable, Category = "Octree")
	void CreateOctreeSpatial(FVector scale, FVector origin, int32 depth);

	UFUNCTION(BlueprintCallable, Category = "Octree")
	void CreateStaticMeshMap(const UWorld* world);

	UOctreeNode* FindNode(const FVector& targetLocation) const;
	UOctreeNode* FindNode(const FVector& leftTopFront, const FVector& rightBottomBack) const;

	void ActiveStaticMeshToLocation(const FVector& leftTopFront, const FVector& rightBottomBack);

private:
	void StaticMeshMapping(TArray<AActor*> actors);
	void AllStaticMeshUnActive();

private:
	TObjectPtr<UOctree> _octree;
	TMap<FString, TArray<UStaticMeshComponent*>> _staticMeshMap;
};
