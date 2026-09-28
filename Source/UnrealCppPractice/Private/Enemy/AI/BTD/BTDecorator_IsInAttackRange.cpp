// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTD/BTDecorator_IsInAttackRange.h"
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
    NodeName = TEXT("Is In Attack Range");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return false;
    }

    const APawn* EnemyPawn = AIController->GetPawn();
    if (!EnemyPawn)
    {
        return false;
    }

    const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return false;
    }

    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
    if (!TargetActor)
    {
        return false;
    }

    const float Distance = FVector::Dist(EnemyPawn->GetActorLocation(), TargetActor->GetActorLocation());

    return Distance <= AttackRange;
}
