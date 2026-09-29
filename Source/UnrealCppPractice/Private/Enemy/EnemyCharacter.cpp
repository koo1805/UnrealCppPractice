// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyCharacter.h"
#include <Enemy/EnemyAIController.h>
#include <Animation/AnimInstance.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/CharacterMovementComponent.h>

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	// AI Controller
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
}

void AEnemyCharacter::Attack()
{
	if (!AttackMontage)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!AnimInstance)
	{
		return;
	}

	// 새로운 공격이므로 데미지 여부 초기화
	bHasAppliedAttackDamage = false;

	AnimInstance->Montage_Play(AttackMontage);
}

void AEnemyCharacter::ApplyAttackDamage()
{
	if (bHasAppliedAttackDamage)
	{
		return;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		return;
	}

	const float Distance = FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation());
	if (Distance > AttackRange)
	{
		return;
	}

	UGameplayStatics::ApplyDamage(PlayerPawn, AttackDamage, GetController(), this, UDamageType::StaticClass());

	bHasAppliedAttackDamage = true;

	UE_LOG(LogTemp, Log, TEXT("Enemy Attack: %.1f Damage"), AttackDamage);
}
