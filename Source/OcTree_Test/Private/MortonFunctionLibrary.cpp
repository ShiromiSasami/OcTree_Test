// Fill out your copyright notice in the Description page of Project Settings.


#include "MortonFunctionLibrary.h"

uint64 UMortonFunctionLibrary::EncodeMorton3D(const FVector& location)
{
	// 単位ベクトル(UnitVec)基準(正規化)の粒度での各座標のビット値に一定の間隔を加える
	const auto nx = static_cast<uint32>(FMath::FloorToInt(location.X));
	const auto ny = static_cast<uint32>(FMath::FloorToInt(location.Y));
	const auto nz = static_cast<uint32>(FMath::FloorToInt(location.Z));

	// デバッグ用
	// auto nxBit = std::bitset<32>(nx).to_string();
	// auto nyBit = std::bitset<32>(ny).to_string();
	// auto nzBit = std::bitset<32>(nz).to_string();
	//
	// auto nxMortonBit = std::bitset<32>(MortonBitSpaceOrganize(nx)).to_string();
	// auto nyMortonBit = std::bitset<32>(MortonBitSpaceOrganize(ny)).to_string();
	// auto nzMortonBit = std::bitset<32>(MortonBitSpaceOrganize(nz)).to_string();
	
	// ZYXのビット並びにして、モートンコードを生成
	return MortonBitSpaceOrganize(nz) << 2 | MortonBitSpaceOrganize(ny) << 1 | MortonBitSpaceOrganize(nx);
}
