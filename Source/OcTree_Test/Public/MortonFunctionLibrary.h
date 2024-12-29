// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MortonFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class OCTREE_TEST_API UMortonFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// 座標 (x, y, z) をモートンコードに変換
	UFUNCTION(Category = "Morton")
	static uint64 EncodeMorton3D(const FVector& location);

	/// <summary>
	/// ビットをモートンコード用に2ビット間隔の整理
	/// </summary>
	/// <param name="rawBit">整理前ビット</param>
	/// <returns>整理後ビット</returns>
	static constexpr uint32 MortonBitSpaceOrganize(const uint32 rawBit)
	{
		auto result = rawBit;
		result = (result | (result << 16)) & 0x00FF00FF;
		result = (result | (result << 8)) & 0x0300F00F;
		result = (result | (result << 4)) & 0x030c30c3;
		result = (result | (result << 2)) & 0x09249249;
		return result;
	}
};
