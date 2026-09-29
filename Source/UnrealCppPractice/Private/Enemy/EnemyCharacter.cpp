// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyCharacter.h"
#include <Enemy/EnemyAIController.h>
#include <Enemy/Weapon/EnemyWeapon.h>
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

	SpawnWeapon();
}

bool AEnemyCharacter::Attack()
{
	if (!AttackMontage)
	{
		return false;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!AnimInstance)
	{
		return false;
	}

	// 이미 공격 몽타주가 재생중이면 건너뜀
	if (AnimInstance->Montage_IsPlaying(AttackMontage))
	{
		return false;
	}

	// 새로운 공격이므로 데미지 여부 초기화
	bHasAppliedAttackDamage = false;

	const float MontageLength = AnimInstance->Montage_Play(AttackMontage);

	return MontageLength > 0.0f;
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

void AEnemyCharacter::SpawnWeapon()
{
	if (!WeaponClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponClass is nullptr"));

		return;
	}

	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;

	// 무기의 Owner를 자신으로 지정
	SpawnParams.Owner = this;

	// Instigator도 자신으로 지정
	SpawnParams.Instigator = this;

	Weapon = World->SpawnActor<AEnemyWeapon>(WeaponClass, GetActorTransform(), SpawnParams);

	if (!Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon is nullptr"));
		return;
	}

	// Skeleton의 Weapon Socket에 부착
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
}
