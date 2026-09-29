// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class UBehaviorTree;
class UAnimMontage;

UCLASS()
class UNREALCPPPRACTICE_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

	// 공격
	void Attack();

	// AnimNotify
	void ApplyAttackDamage();

protected:
	// 최대 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Stats")
	float MaxHealth = 100.0f;

	// 현재 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Stats")
	float CurrentHealth = 0.0f;

	// 비헤이비어 트리
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	// 공격 애니메이션
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Combat")
	TObjectPtr<UAnimMontage> AttackMontage;

	// 공격력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Combat")
	float AttackDamage = 20.0f;

	// 공격 판정 거리
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Combat")
	float AttackRange = 150.0f;

private:
	// 현재 공격에 이미 데미지를 적용했는지
	bool bHasAppliedAttackDamage = false;
};
