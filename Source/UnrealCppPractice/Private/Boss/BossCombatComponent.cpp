// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossCombatComponent.h"
#include <Boss/BossCharacter.h>
#include <Boss/Data/BossCombatDataAsset.h>
#include <Boss/Weapon/BossGreatSword.h>
#include <Animation/AnimInstance.h>
#include <Animation/AnimMontage.h>

// Sets default values for this component's properties
UBossCombatComponent::UBossCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UBossCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// 이 Component를 가지고 있는 Actor를 BossCharacter로 변환
	OwnerBoss = Cast<ABossCharacter>(GetOwner());
	
	if (!OwnerBoss)
	{
		UE_LOG(LogTemp, Error, TEXT("BossCombatComponent Owner is not BossCharacter"));
	}
}

bool UBossCombatComponent::StartAttack(EBossAttackType AttackType)
{
	// Boss가 없으면 공격 불가능
	if (!OwnerBoss)
	{
		return false;
	}

	// 죽은 상태에서는 공격 불가능
	if (OwnerBoss->IsDead())
	{
		return false;
	}

	// 경직 상태에서는 공격 불가능
	if (OwnerBoss->IsStaggered())
	{
		return false;
	}

	// 이미 다른 공격을 실행 중이면 공격 불가능
	if (bIsAttacking)
	{
		return false;
	}

	// None은 실제 공격이 아니다.
	if (AttackType == EBossAttackType::None)
	{
		return false;
	}

	// Cooldown 검사
	if (!IsAttackReady(AttackType))
	{
		return false;
	}

	const UBossCombatDataAsset* CombatData = GetCombatData();

	if (!CombatData)
	{
		return false;
	}

	// DataAsset에서 해당 공격 검색
	const FBossAttackData* AttackData = CombatData->FindAttackData(AttackType);

	if (!AttackData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack Data Not Found"));

		return false;
	}

	// Montage가 설정되어 있는지 확인
	if (!AttackData->AttackMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack Montage is nullptr"));

		return false;
	}

	// Montage 실행
	UAnimInstance* AnimInstance = OwnerBoss->GetMesh()->GetAnimInstance();

	if (!AnimInstance)
	{
		return false;
	}

	const float MontageLength = AnimInstance->Montage_Play(AttackData->AttackMontage);

	// Montage_Play 실패
	if (MontageLength <= 0.0f)
	{
		return false;
	}

	// 공격 상태 설정
	bIsAttacking = true;

	CurrentAttackType = AttackType;

	CurrentAttackData = AttackData;

	// Cooldown 시작 시간 기록
	if (UWorld* World = GetWorld())
	{
		LastAttackTimes.Add(AttackType, World->GetTimeSeconds());
	}

	// Montage 종료 Delegate 등록
	FOnMontageEnded MontageEndedDelegate;

	MontageEndedDelegate.BindUObject(this, &UBossCombatComponent::OnAttackMontageEnded);

	AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackData->AttackMontage);

	UE_LOG(LogTemp, Log, TEXT("Boss Attack Start: %d"), static_cast<int32>(AttackType));

	return true;
}

void UBossCombatComponent::EndAttack()
{
	if (!bIsAttacking)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Boss Attack End"));

	bIsAttacking = false;

	CurrentAttackType = EBossAttackType::None;

	CurrentAttackData = nullptr;
}

void UBossCombatComponent::CancelAttack()
{
	if (!bIsAttacking)
	{
		return;
	}

	// 공격 판정이 켜져 있다면 강제로 종료
	if (OwnerBoss)
	{
		if (ABossGreatSword* Sword = OwnerBoss->GetGreatSword())
		{
			Sword->DisableAttackCollision();
		}

		UAnimInstance* AnimInstance = OwnerBoss->GetMesh()->GetAnimInstance();

		if (AnimInstance && CurrentAttackData)
		{
			if (CurrentAttackData->AttackMontage)
			{
				AnimInstance->Montage_Stop(0.2f, CurrentAttackData->AttackMontage);
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Boss Attack Cancel"));

	// Montage_Stop()은 종료 Delegate를 발생시키지만 여기서도 즉시 상태를 정리
	EndAttack();
}

EBossAttackType UBossCombatComponent::SelectCloseAttack() const
{
	const UBossCombatDataAsset* CombatData = GetCombatData();

	if (!CombatData)
	{
		return EBossAttackType::None;
	}

	// 근거리에서 사용할 공격 종류
	const TArray<EBossAttackType> CloseAttackTypes =
	{
		EBossAttackType::Normal,
		EBossAttackType::Combo,
		EBossAttackType::DelayedHeavy
	};

	float TotalWeight = 0.0f;

	// 전체 Weight 계산
	for (const EBossAttackType AttackType : CloseAttackTypes)
	{
		const FBossAttackData* AttackData = CombatData->FindAttackData(AttackType);

		if (!AttackData)
		{
			continue;
		}

		// Cooldown 중인 공격은 선택 대상에서 제외
		if (!IsAttackReady(AttackType))
		{
			continue;
		}

		if (AttackData->Weight <= 0.0f)
		{
			continue;
		}

		TotalWeight += AttackData->Weight;
	}


	if (TotalWeight <= 0.0f)
	{
		return EBossAttackType::None;
	}

	// Weight 기반 Random 값 생성
	const float RandomValue = FMath::FRandRange(0.0f, TotalWeight);

	float AccumulatedWeight = 0.0f;

	// 실제 공격 선택
	for (const EBossAttackType AttackType : CloseAttackTypes)
	{
		const FBossAttackData* AttackData = CombatData->FindAttackData(AttackType);

		if (!AttackData)
		{
			continue;
		}

		if (!IsAttackReady(AttackType))
		{
			continue;
		}

		if (AttackData->Weight <= 0.0f)
		{
			continue;
		}

		AccumulatedWeight += AttackData->Weight;

		if (RandomValue <= AccumulatedWeight)
		{
			return AttackType;
		}
	}

	return EBossAttackType::None;
}

bool UBossCombatComponent::IsAttackReady(EBossAttackType AttackType) const
{
	const UBossCombatDataAsset* CombatData = GetCombatData();

	if (!CombatData)
	{
		return false;
	}

	const FBossAttackData* AttackData = CombatData->FindAttackData(AttackType);

	if (!AttackData)
	{
		return false;
	}

	// 사용한 적이 없는 공격
	const float* LastAttackTime = LastAttackTimes.Find(AttackType);

	if (!LastAttackTime)
	{
		return true;
	}

	const UWorld* World = GetWorld();

	if (!World)
	{
		return false;
	}

	const float CurrentTime = World->GetTimeSeconds();

	const float ElapsedTime = CurrentTime - *LastAttackTime;

	return ElapsedTime >= AttackData->Cooldown;
}

const UBossCombatDataAsset* UBossCombatComponent::GetCombatData() const
{
	if (!OwnerBoss)
	{
		return nullptr;
	}

	return OwnerBoss->GetCombatData();
}

void UBossCombatComponent::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bIsAttacking)
	{
		return;
	}

	if (!CurrentAttackData)
	{
		EndAttack();
		return;
	}

	// 현재 공격 Montage의 종료인지 확인
	if (Montage != CurrentAttackData->AttackMontage)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Attack Montage End - Interrupted: %s"), bInterrupted ? TEXT("True") : TEXT("False"));

	EndAttack();
}
