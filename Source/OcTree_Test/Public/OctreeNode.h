// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OctreeNode.generated.h"

/**
 * 
 */
UCLASS()
class OCTREE_TEST_API UOctreeNode : public UObject
{
	GENERATED_BODY()

public:
	UOctreeNode();
	virtual ~UOctreeNode() override;

	/// <summary>
	/// 8つの子ノードを作成
	/// </summary>
	/// <param name="size">サイズ</param>
	/// <param name="origin">原点</param>
	/// <param name="parent">親ノード</param>
	/// <param name="depth">深さ</param>
	/// <param name="id">ID</param>
	void CreateChildren(const FVector& size, const FVector& origin, UOctreeNode* parent = nullptr, const uint32 depth = 0, const uint64 id = 0);

	/// <summary>
	/// ノードを作成
	/// </summary>
	/// <param name="locationMortonCode">親ノード</param>
	bool MortonCodeCheck(uint64 locationMortonCode) const;

	/// <summary>
	/// ノードを探索
	/// </summary>
	/// <param name="mortonCode">対象のモートンコード</param>
	/// <param name="finishDepth">探索する最大の深さ</param>
	/// <returns>ノード</returns>w
	UOctreeNode* FindNode(uint64 mortonCode, const uint32 finishDepth = std::numeric_limits<uint32>::max());

#pragma region Setter/Getter

	static void SetMaxDepth(const uint16 depth) { MaxDepth = depth; }
	FORCEINLINE void SetMask(const uint64 mask) { _mask = mask; }

	FORCEINLINE uint64 GetId() const { return _id; }
	FORCEINLINE FVector GetSize() const { return _size; }
	FORCEINLINE FVector GetOrigin() const { return _origin; }
	FORCEINLINE uint32 GetDepth() const { return _depth; }
	FORCEINLINE TArray<UOctreeNode*> GetChildren() const{ return _children; }
	
#pragma endregion
	
private:
	
	static constexpr uint8 ChildCount = 8;

	static uint16 MaxDepth;
	
	TWeakObjectPtr<UOctreeNode> _parent;
	FVector _size;
	FVector _origin;
	uint32 _depth;
	uint32 _id;
	uint32 _mask;
	UPROPERTY()
	TArray<TObjectPtr<UOctreeNode>> _children;
};
