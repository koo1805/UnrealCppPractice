// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTT/BTTask_Attack.h"
#include <AIController.h>
#include <Enemy/EnemyCharacter.h>

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>((AIController->GetPawn()));
	if (!Enemy)
	{
		return EBTNodeResult::Failed;
	}

	Enemy->Attack();

	return EBTNodeResult::Succeeded;
}
