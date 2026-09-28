// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class UBehaviorTree;

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
};
