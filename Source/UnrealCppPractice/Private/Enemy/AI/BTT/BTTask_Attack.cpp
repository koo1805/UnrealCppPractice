// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTT/BTTask_Attack.h"
#include <AIController.h>
#include <Animation/AnimInstance.h>
#include <Enemy/EnemyCharacter.h>

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");

	// Enemy마다 Task상태를 따로 가지도록함
	bCreateNodeInstance = true;
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

	UAnimInstance* AnimInstance = Enemy->GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return EBTNodeResult::Failed;
	}

	CachedOwnerComp = &OwnerComp;
	CachedEnemy = Enemy;

	// 공격 시작
	if (!Enemy->Attack())
	{
		CachedOwnerComp.Reset();
		CachedEnemy.Reset();

		return EBTNodeResult::Failed;
	}

	// 몽타주 종료 이벤트 등록
	FOnMontageEnded MontageEndedDelegate;
	MontageEndedDelegate.BindUObject(this, &UBTTask_Attack::OnAttackMontageEnded);

	AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, Enemy->GetAttackMontage());

	// 아직 공격 중
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (CachedEnemy.IsValid())
	{
		if (UAnimInstance* AnimInstance = CachedEnemy->GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Stop(0.2f);
		}
	}

	CachedOwnerComp.Reset();
	CachedEnemy.Reset();

	return EBTNodeResult::Type();
}

// 몽타주 종료 처리
void UBTTask_Attack::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!CachedOwnerComp.IsValid())
	{
		return;
	}

	UBehaviorTreeComponent* OwnerComp = CachedOwnerComp.Get();

	FinishLatentTask(*OwnerComp, bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded);

	CachedOwnerComp.Reset();
	CachedEnemy.Reset();
}
