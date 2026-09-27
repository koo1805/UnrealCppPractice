// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/AI/BTS/BTService_UpdateBossState.h"
//#include "BTService_UpdateBossState.h"
#include <Boss/BossAIController.h>
#include <Boss/BossCharacter.h>
#include <Boss/BossCombatComponent.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BlackboardComponent.h>

UBTService_UpdateBossState::UBTService_UpdateBossState()
{
	NodeName = TEXT("Update Boss State");

	// Service 실행 주기
	Interval = 0.2f;

	// 실행 주기의 랜덤 편차
	RandomDeviation = 0.0f;
}

void UBTService_UpdateBossState::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// AI Controller
	ABossAIController* AIController = Cast<ABossAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		return;
	}

	// Boss Character
	ABossCharacter* Boss = AIController->GetBossCharacter();
	if (!Boss)
	{
		return;
	}

	// Blackboard
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
	{
		return;
	}

	// Target
	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(ABossAIController::TargetActorKey));

	// Distance
	if (TargetActor)
	{
		const float Distance = FVector::Dist(Boss->GetActorLocation(), TargetActor->GetActorLocation());

		Blackboard->SetValueAsFloat(ABossAIController::DistanceToTargetKey, Distance);
	}
	else
	{
		// Target이 없으면 거리 판단을 하지 못하도록 매우 큰 값으로 설정
		Blackboard->SetValueAsFloat(ABossAIController::DistanceToTargetKey, TNumericLimits<float>::Max());
	}

	// Phase
	Blackboard->SetValueAsInt(ABossAIController::PhaseKey, Boss->GetCurrentPhase());

	// Stagger
	Blackboard->SetValueAsBool(ABossAIController::IsStaggeredKey, Boss->IsStaggered());

	// Dead
	Blackboard->SetValueAsBool(ABossAIController::IsDeadKey, Boss->IsDead());

	// Combat
	UBossCombatComponent* CombatComponent = Boss->GetCombatComponent();

	Blackboard->SetValueAsBool(ABossAIController::IsAttackingKey, CombatComponent ? CombatComponent->IsAttacking() : false);
}
