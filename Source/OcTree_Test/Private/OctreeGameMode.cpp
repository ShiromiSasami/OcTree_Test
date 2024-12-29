// Fill out your copyright notice in the Description page of Project Settings.


#include "OctreeGameMode.h"

#include "Octree.h"
#include "OctreeNode.h"
#include "OctreeSpatialDivisionHandler.h"
#include "Kismet/KismetSystemLibrary.h"

void AOctreeGameMode::BeginPlay()
{
	Super::BeginPlay();

	_handler = NewObject<UOctreeSpatialDivisionHandler>();
	_handler->CreateOctreeSpatial(FVector(1000.0f, 1000.0f, 1000.0f), FVector(1200.0f, 1700.0f, 500.0f), 5);
	_handler->CreateStaticMeshMap(GetWorld());
}

void AOctreeGameMode::CheckOctreeNode(const AActor* targetActor) const
{
	if (!_handler)
	{
		return;
	}

	// if (auto node = _handler->FindNode(targetActor->GetActorLocation()))
	// {
	// 	UKismetSystemLibrary::DrawDebugBox(this, node->GetOrigin(), node->GetSize(), FColor::Red, FRotator::ZeroRotator, 0.1f, 2);
	// 	UE_LOG(LogTemp, Warning, TEXT("Node Location: %d, %d, %d"), (int)node->GetOrigin().X, (int)node->GetOrigin().Y, (int)node->GetOrigin().Z);
	// }

	const auto leftTopFront = targetActor->GetActorLocation() + FVector(-50.0f, 50.0f, 50.0f);
	const auto rightBottomBack = targetActor->GetActorLocation() + FVector(50.0f, -50.0f, -50.0f);
	
	_handler->ActiveStaticMeshToLocation(leftTopFront, rightBottomBack);
	
	UKismetSystemLibrary::DrawDebugSphere(this, leftTopFront, 5, 10, FColor::Blue, 0.1f, 2);
	UKismetSystemLibrary::DrawDebugSphere(this, rightBottomBack, 5, 10, FColor::Blue, 0.1f, 2);
	if (auto node = _handler->FindNode(leftTopFront, rightBottomBack))
	{
		UKismetSystemLibrary::DrawDebugBox(this, node->GetOrigin(), node->GetSize(), FColor::Blue, FRotator::ZeroRotator, 0.1f, 2);
	}
}

