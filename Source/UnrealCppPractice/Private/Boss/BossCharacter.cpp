// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossCharacter.h"
#include <Boss/Data/BossCombatDataAsset.h>
#include <Boss/BossCombatComponent.h>
#include <Boss/Weapon/BossGreatSword.h>

// Sets default values
ABossCharacter::ABossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CombatComponent = CreateDefaultSubobject<UBossCombatComponent>(TEXT("BossCombatComponent"));
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// 게임 시작 시 현재 HP를 최대 HP로 초기화
	CurrentHP = MaxHP;

	// 경직치 초기화
	CurrentStagger = 0.0f;

	// 시작 페이즈
	CurrentPhase = 1;

	bIsStaggered = false;
	bIsDead = false;

	SpawnGreatSword();

	UE_LOG(LogTemp, Log, TEXT("Boss Start HP: %.1f"), CurrentHP);

	if (CombatComponent)
	{
		CombatComponent->StartAttack(EBossAttackType::Normal);
	}
}

// 데미지
float ABossCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (bIsDead)
	{
		return 0.0f;
	}

	if (ActualDamage <= 0.0f)
	{
		return 0.0f;
	}

	CurrentHP = FMath::Clamp(CurrentHP - ActualDamage, 0.0f, MaxHP);

	UE_LOG(LogTemp, Log, TEXT("Boss Damage: %.1f / HP: %.1f / %.1f"), ActualDamage, CurrentHP, MaxHP);

	if (CurrentHP <= 0.0f)
	{
		Die();

		return ActualDamage;
	}

	CheckPhaseTransition();

	return ActualDamage;
}

// 경직
void ABossCharacter::AddStagger(float StaggerAmount)
{
	if (bIsDead)
	{
		return;
	}

	if (bIsStaggered)
	{
		return;
	}

	if (StaggerAmount <= 0.0f)
	{
		return;
	}

	CurrentStagger = FMath::Clamp(CurrentStagger + StaggerAmount, 0.0f, MaxStagger);

	if (CurrentStagger >= MaxStagger)
	{
		EnterStagger();
	}
}

// 페이즈
void ABossCharacter::CheckPhaseTransition()
{
	if (CurrentPhase != 1)
	{
		return;
	}

	if (!CombatData)
	{
		return;
	}

	if (MaxHP <= 0.0f)
	{
		return;
	}

	const float HPRatio = CurrentHP / MaxHP;

	if (HPRatio <= CombatData->Phase2HPThreshold)
	{
		EnterPhase2();
	}
}

void ABossCharacter::EnterPhase2()
{
	if (CurrentPhase == 2)
	{
		return;
	}

	CurrentPhase = 2;

	UE_LOG(LogTemp, Log, TEXT("Boss entered Phase 2"));

	// 이후 단계에서 페이즈 전환 Animation / VFX / BT 상태 변경 등을 연결
}

// ==========================================================
// 경직
void ABossCharacter::EnterStagger()
{
	if (bIsDead)
	{
		return;
	}

	if (bIsStaggered)
	{
		return;
	}

	bIsStaggered = true;

	CurrentStagger = 0.0f;

	// 공격 중이었다면 현재 공격 취소
	if (CombatComponent)
	{
		CombatComponent->CancelAttack();
	}

	UE_LOG(LogTemp, Log, TEXT("Boss Staggered"));

	// 이후 단계에서 Stagger Montage를 연결
}

void ABossCharacter::ExitStagger()
{
	if (!bIsStaggered)
	{
		return;
	}

	bIsStaggered = false;

	UE_LOG(LogTemp, Log, TEXT("Boss Stagger End"));
}

void ABossCharacter::Die()
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;

	// 사망 시 현재 공격 취소
	if (CombatComponent)
	{
		CombatComponent->CancelAttack();
	}

	UE_LOG(LogTemp, Log, TEXT("Boss Dead"));
}

void ABossCharacter::SpawnGreatSword()
{
	if (!GreatSwordClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("GreatSwordClass is nullptr"));

		return;
	}


	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;

	// Sword의 Owner를 Boss로 지정
	SpawnParams.Owner = this;

	// Instigator도 Boss로 지정
	SpawnParams.Instigator = this;

	GreatSword = World->SpawnActor<ABossGreatSword>(GreatSwordClass, GetActorTransform(), SpawnParams);

	if (!GreatSword)
	{
		return;
	}

	// Boss Skeleton의 Weapon Socket에 부착
	GreatSword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
}
