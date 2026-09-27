// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossCharacter.generated.h"

class UBossCombatDataAsset;

UCLASS()
class UNREALCPPPRACTICE_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// 데미지
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// 경직
	// 보스의 경직치를 증가
	void AddStagger(float StaggerAmount);

	// 현재 경직 상태인지 반환
	bool IsStaggered() const { return bIsStaggered; }

	// 상태 Getter
	float GetCurrentHP() const { return CurrentHP; }
	float GetMaxHP() const { return MaxHP; }

	float GetCurrentStagger() const { return CurrentStagger; }
	float GetMaxStagger() const { return MaxStagger; }

	int32 GetCurrentPhase() const { return CurrentPhase; }

	bool IsDead() const { return bIsDead; }

	const UBossCombatDataAsset* GetCombatData() const { return CombatData; }

protected:
	// HP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss|Stat")
	float MaxHP = 1000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss|Stat")
	float CurrentHP = 0.0f;

	// 경직
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss|Stat")
	float MaxStagger = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss|Stat")
	float CurrentStagger = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss|State")
	bool bIsStaggered = false;

	// 페이즈
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss|State")
	int32 CurrentPhase = 1;

	// 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss|State")
	bool bIsDead = false;

	// 데이터
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss|Data")
	TObjectPtr<UBossCombatDataAsset> CombatData;

protected:
	// HP가 Phase 전환 조건을 만족했는지 검사
	void CheckPhaseTransition();

	// 2페이즈 진입
	void EnterPhase2();

	// 경직 발생
	void EnterStagger();

	// 경직 종료
	void ExitStagger();

	// 사망 처리
	void Die();
};
