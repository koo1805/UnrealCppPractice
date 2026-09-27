// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAIController.generated.h"

class UBehaviorTree;
class UBlackboardComponent;
class ABossCharacter;

/**
 * 
 */
UCLASS()
class UNREALCPPPRACTICE_API ABossAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ABossAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

public:
	// Getter
	ABossCharacter* GetBossCharacter() const { return BossCharacter; }

private:
	// Blackboard 초기값 설정
	void InitializeBlackboardValues();

	// TargetActor 초기 설정
	void InitializeTarget();

public:
	// Blackboard Key Names
	static const FName TargetActorKey;

	static const FName DistanceToTargetKey;

	static const FName PhaseKey;

	static const FName IsAttackingKey;

	static const FName IsStaggeredKey;

	static const FName IsDeadKey;

protected:
	// 실행할 Behavior Tree
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss|AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	// Boss
	UPROPERTY()
	TObjectPtr<ABossCharacter> BossCharacter;
};
