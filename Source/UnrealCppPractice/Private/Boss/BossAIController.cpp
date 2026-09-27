// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossAIController.h"
//#include "BossAIController.h"
#include <Boss/BossCharacter.h>
#include <Boss/BossCombatComponent.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Kismet/GameplayStatics.h>

// Blackboard Key Names
const FName ABossAIController::TargetActorKey(TEXT("TargetActor"));

const FName ABossAIController::DistanceToTargetKey(TEXT("DistanceToTarget"));

const FName ABossAIController::PhaseKey(TEXT("Phase"));

const FName ABossAIController::IsAttackingKey(TEXT("IsAttacking"));

const FName ABossAIController::IsStaggeredKey(TEXT("IsStaggered"));

const FName ABossAIController::IsDeadKey(TEXT("IsDead"));

ABossAIController::ABossAIController()
{}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Boss Character 확인
	BossCharacter = Cast<ABossCharacter>(InPawn);

	if (!BossCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("BossAIController possessed invalid Pawn"));
		return;
	}

	// Behavior Tree 확인
	if (!BehaviorTreeAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("BehaviorTreeAsset is nullptr"));
		return;
	}

	// Blackboard 초기화
	if (!BehaviorTreeAsset->BlackboardAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("BehaviorTree has no Blackboard Asset"));
		return;
	}

	UBlackboardComponent* BlackboardComponent = nullptr;
	if (!UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("UseBlackboard Failed"));
		return;
	}

	// 초기 Blackboard 값
	InitializeBlackboardValues();

	InitializeTarget();

	// Behavior Tree 실행
	if (!RunBehaviorTree(BehaviorTreeAsset))
	{
		UE_LOG(LogTemp, Error, TEXT("RunBehaviorTree Failed"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Boss Behavior Tree Started"));
}

void ABossAIController::InitializeBlackboardValues()
{
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();

	if (!BlackboardComp)
	{
		return;
	}

	BlackboardComp->SetValueAsFloat(DistanceToTargetKey, 0.0f);

	BlackboardComp->SetValueAsInt(PhaseKey, BossCharacter->GetCurrentPhase());

	UBossCombatComponent* CombatComponent = BossCharacter->GetCombatComponent();

	BlackboardComp->SetValueAsBool(IsAttackingKey, CombatComponent ? CombatComponent->IsAttacking() : false);

	BlackboardComp->SetValueAsBool(IsStaggeredKey, BossCharacter->IsStaggered());

	BlackboardComp->SetValueAsBool(IsDeadKey, BossCharacter->IsDead());
}

void ABossAIController::InitializeTarget()
{
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();

	if (!BlackboardComp)
	{
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);

	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController 0 Not Found"));
		return;
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("PlayerController Found: %s"),
		*PC->GetName()
	);

	APawn* PlayerPawnt = PC->GetPawn();

	if (!PlayerPawnt)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("PlayerController exists, but PlayerPawn is None")
		);
		return;
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("PlayerPawn Found: %s"),
		*PlayerPawnt->GetName()
	);

	// 현재 프로젝트는 Single Player Boss를 기준으로 Player 0을 초기 Target으로 설정
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	UObject* StoredTarget = BlackboardComp->GetValueAsObject(TargetActorKey);

	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Character Not Found"));
		return;
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("PlayerPawn Found: %s / Class: %s"),
		*PlayerPawn->GetName(),
		*PlayerPawn->GetClass()->GetName()
	);

	BlackboardComp->SetValueAsObject(TargetActorKey, PlayerPawn);

	// 초기 거리도 바로 계산
	const float Distance = FVector::Dist(BossCharacter->GetActorLocation(), PlayerPawn->GetActorLocation());

	BlackboardComp->SetValueAsFloat(DistanceToTargetKey, Distance);

	UE_LOG(LogTemp, Log, TEXT("Boss Target Initialized: %s"), *PlayerPawn->GetName());
}