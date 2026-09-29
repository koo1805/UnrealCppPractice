// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCPPPRACTICE_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Attack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	// 현재 이 Task를 실행중인 비헤이비어트리 컴포넌트
	TWeakObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;

	// 현재 공격중인 Enemy
	TWeakObjectPtr<class AEnemyCharacter> CachedEnemy;
};
