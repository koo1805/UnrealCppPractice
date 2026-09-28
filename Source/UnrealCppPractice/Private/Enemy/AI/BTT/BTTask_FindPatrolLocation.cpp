// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTT/BTTask_FindPatrolLocation.h"
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <NavigationSystem.h>

UBTTask_FindPatrolLocation::UBTTask_FindPatrolLocation()
{
	NodeName = TEXT("Find Patrol Location");
}

EBTNodeResult::Type UBTTask_FindPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Behavior Tree를 실행하고 있는 AIController 획득
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	// AIController가 제어하고 있는 Enemy 획득
	APawn* EnemyPawn = AIController->GetPawn();
	if (!EnemyPawn)
	{
		return EBTNodeResult::Failed;
	}

	// Navigation System 획득
	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(EnemyPawn->GetWorld());
	if (!NavigationSystem)
	{
		return EBTNodeResult::Failed;
	}

	// Enemy 현재 위치
	const FVector Origin = EnemyPawn->GetActorLocation();

	//NavMesh 위의 랜덤 위치를 받을 수 있는 변수
	FNavLocation RandomLocation;

	// 현재 위치를 기준으로 도달 가능한 랜덤 위치 검색
	const bool bFoundLocation = NavigationSystem->GetRandomReachablePointInRadius(Origin, PatrolRadius, RandomLocation);
	if (!bFoundLocation)
	{
		return EBTNodeResult::Failed;
	}

	// Blackboard 획득
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return EBTNodeResult::Failed;
	}

	// 찾은 위치를 PatrolLocation에 저장
	BlackboardComp->SetValueAsVector(TEXT("PatrolLocation"), RandomLocation.Location);

	return EBTNodeResult::Succeeded;
}
