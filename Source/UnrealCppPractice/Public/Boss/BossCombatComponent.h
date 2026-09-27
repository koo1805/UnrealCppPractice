// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Boss/Data/BossCombatTypes.h>
#include "BossCombatComponent.generated.h"

class ABossCharacter;
class UBossCombatDataAsset;
class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALCPPPRACTICE_API UBossCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// 지정한 공격을 시작
	// 공격 시작 성공 여부를 반환
	bool StartAttack(EBossAttackType AttackType);

	// 현재 공격을 종료
	void EndAttack();

	// 현재 공격을 강제로 취소
	void CancelAttack();

	// Normal / Combo / DelayedHeavy 중
	// 가중치를 이용해 공격 하나를 선택
	EBossAttackType SelectCloseAttack() const;

	// 해당 공격을 현재 사용할 수 있는지 검사
	bool IsAttackReady(EBossAttackType AttackType) const;

	// Getter
	bool IsAttacking() const { return bIsAttacking; }

	EBossAttackType GetCurrentAttackType() const { return CurrentAttackType; }

	const FBossAttackData* GetCurrentAttackData() const { return CurrentAttackData; }

private:
	// DataAsset 반환
	const UBossCombatDataAsset* GetCombatData() const;

	// Montage 종료 Callback
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	// 이 Component를 가지고 있는 Boss
	UPROPERTY()
	TObjectPtr<ABossCharacter> OwnerBoss;

	// 현재 공격 중인지
	bool bIsAttacking = false;

	// 현재 실행 중인 공격 종류
	EBossAttackType CurrentAttackType = EBossAttackType::None;

	// 현재 공격 데이터
	const FBossAttackData* CurrentAttackData = nullptr;

	// 공격별 마지막 사용 시간을 저장
	// Key   = AttackType | Value = 마지막 공격 시작 시간
	TMap<EBossAttackType, float> LastAttackTimes;
};
