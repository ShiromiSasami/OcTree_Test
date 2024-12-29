// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OctreeGameMode.generated.h"

class UOctreeSpatialDivisionHandler;

/**
 * 
 */
UCLASS()
class OCTREE_TEST_API AOctreeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Octree")
	void CheckOctreeNode(const AActor* targetActor) const;

private:
	TObjectPtr<UOctreeSpatialDivisionHandler> _handler = nullptr;
};
