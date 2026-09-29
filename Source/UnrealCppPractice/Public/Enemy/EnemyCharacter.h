// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class UBehaviorTree;
class UAnimMontage;
class AEnemyWeapon;

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

	UAnimMontage* GetAttackMontage() const { return AttackMontage; }

	// 공격
	bool Attack();

	// AnimNotify
	UFUNCTION(BlueprintCallable, Category = "Enemy|Combat")
	void ApplyAttackDamage();

	// 무기 장착
	void SpawnWeapon();

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

	// 무기 클래스
	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Weapon")
	TSubclassOf<AEnemyWeapon> WeaponClass;

	// 현재 적이 장착한 무기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Weapon")
	TObjectPtr<AEnemyWeapon> Weapon;


private:
	// 현재 공격에 이미 데미지를 적용했는지
	bool bHasAppliedAttackDamage = false;
};
