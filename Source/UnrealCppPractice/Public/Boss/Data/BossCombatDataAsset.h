// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BossCombatTypes.h"
#include "BossCombatDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UNREALCPPPRACTICE_API UBossCombatDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// 페이즈
	// HP가 해당 값 이하가 되면 2페이즈 진입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Phase")
	float Phase2HPThreshold = 0.5f;

	// 거리 판단
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Distance")
	FBossDistanceData DistanceData;

	// 공격 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	TArray<FBossAttackData> Attacks;

public:
	// 공격 타입 데이터
	const FBossAttackData* FindAttackData(EBossAttackType AttackType) const;
};
