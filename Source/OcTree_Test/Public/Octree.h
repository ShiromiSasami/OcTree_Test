// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Octree.generated.h"

class UOctreeNode;

/**
 * 
 */
UCLASS()
class OCTREE_TEST_API UOctree : public UObject
{
	GENERATED_BODY()

public:
	
	UOctree();

	/// <summary>
	/// 8つの子ノードを作成
	/// </summary>
	/// <param name="size">サイズ</param>
	/// <param name="origin">原点</param>
	/// <param name="depth">深さ</param>
	void CreateNode(const FVector& size, const FVector& origin, const uint32 depth);

	/// <summary>
	/// ノードを探索
	/// </summary>
	/// <param name="location">座標</param>
	/// <returns>ノード</returns>
	UOctreeNode* FindNode(const FVector& location) const;

	/// <summary>
	/// ノードを探索
	/// </summary>
	/// <param name="leftTopFront">左上前</param>
	/// <param name="rightBottomBack">右下奥</param>
	/// <returns>ノード</returns>
	UOctreeNode* FindNode(const FVector& leftTopFront, const FVector& rightBottomBack);

	/// <summary>
	/// 作成したノードの削除
	/// </summary>
	void DeleteNode();

#pragma region Setter/Getter

	FORCEINLINE FVector GetOrigin() const { return _origin; }
	FORCEINLINE FVector GetSize() const { return _size; }
	FORCEINLINE FVector GetUnitVec() const { return _unitVec; }
	
#pragma endregion

private:
	static constexpr uint32 ChildCount = 8;

	UPROPERTY()
	TObjectPtr<UOctreeNode> _rootNode;
	uint64 _organizeMask;
	uint32 _maxDepth;
	FVector _origin;
	FVector _size;
	FVector _unitVec;
};
