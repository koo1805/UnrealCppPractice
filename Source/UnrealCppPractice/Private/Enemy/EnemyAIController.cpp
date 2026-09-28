// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"

AEnemyAIController::AEnemyAIController()
{
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("Enemy AI Possessed: %s"), *GetNameSafe(InPawn));

	// test
	if (!InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not AI Possessed"));
		return;
	}

	const FVector CurrentLocation = InPawn->GetActorLocation();

	const FVector TestLocation = CurrentLocation + FVector(50.0f, 0.0f, 0.0f);

	MoveToLocation(TestLocation);
}
