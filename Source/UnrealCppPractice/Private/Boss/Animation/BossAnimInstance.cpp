// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Animation/BossAnimInstance.h"
#include <GameFramework/Pawn.h>

void UBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerPawn = TryGetPawnOwner();
}

void UBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!OwnerPawn)
    {
        OwnerPawn = TryGetPawnOwner();

        if (!OwnerPawn)
        {
            return;
        }
    }

    const FVector Velocity = OwnerPawn->GetVelocity();

    // Z축 제외
    Speed = FVector(Velocity.X, Velocity.Y, 0.0f).Size();

    bIsMoving = Speed > 3.0f;
}
