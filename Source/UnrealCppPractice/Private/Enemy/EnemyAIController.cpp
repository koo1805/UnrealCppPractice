// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"
#include <Enemy/EnemyCharacter.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Perception/AIPerceptionComponent.h>
#include <Perception/AISenseConfig_Sight.h>

AEnemyAIController::AEnemyAIController()
{
	// 컴포넌트 생성
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	// 시야 객체 생성
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	// 감지 거리
	SightConfig->SightRadius = 1500.0f;

	// 감지 놓치는 거리
	SightConfig->LoseSightRadius = 1800.0f;

	// 좌우 시야각
	SightConfig->PeripheralVisionAngleDegrees = 60.0f;

	// 마지막으로 본 위치를 기억하는 시간
	SightConfig->SetMaxAge(5.0f);

	// 소속 감지별 감지 여부
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// 컴포넌트에 시야 등록
	AIPerceptionComponent->ConfigureSense(*SightConfig);

	// 기본 Sense를 Sight로 지정
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	// 감지 상태 변경 이벤트 연결
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(InPawn);

	if (!Enemy)
	{
		return;
	}

	UBehaviorTree* BehaviorTree = Enemy->GetBehaviorTree();

	if (!BehaviorTree)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Behavior Tree Not Found"));
		return;
	}

	RunBehaviorTree(BehaviorTree);

	UE_LOG(LogTemp, Warning, TEXT("Enemy Behavior Tree Started"));
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor)
	{
		return;
	}

	if (!Actor->ActorHasTag(TEXT("Player")))
	{
		return;
	}

	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		// 추격 시작
		BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), Actor);
		UE_LOG(LogTemp, Log, TEXT("Player Detected - TargetActor Set"));
	}
	else
	{
		BlackboardComponent->ClearValue(TEXT("TargetActor"));
		UE_LOG(LogTemp, Log, TEXT("Failed Detected - TargetActor Clear"));

		// 추격 종료
	}
}
