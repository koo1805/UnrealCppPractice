// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Weapon/BossGreatSword.h"
#include <Boss/BossCharacter.h>
#include <Boss/BossCombatComponent.h>
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
ABossGreatSword::ABossGreatSword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SwordMesh->SetupAttachment(SceneRoot);
	SwordMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HitCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCollision"));
	HitCollision->SetupAttachment(SwordMesh);

	// 게임 시작 시 공격 판정 비활성화
	HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Overlap 이벤트 사용
	HitCollision->SetGenerateOverlapEvents(true);

	// 기본적으로 모든 Channel 무시
	HitCollision->SetCollisionResponseToAllChannels(ECR_Ignore);

	// Pawn과만 Overlap
	HitCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void ABossGreatSword::BeginPlay()
{
	Super::BeginPlay();
	
	// Owner Boss
	OwnerBoss = Cast<ABossCharacter>(GetOwner());

	if (!OwnerBoss)
	{
		UE_LOG(LogTemp, Error, TEXT("BossGreatSword Owner is not BossCharacter"));
	}

	// Overlap Event
	HitCollision->OnComponentBeginOverlap.AddDynamic(this, &ABossGreatSword::OnWeaponOverlap);
}

void ABossGreatSword::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Actor가 유효하지 않음
	if (!OtherActor)
	{
		return;
	}

	// 자기 자신은 공격하지 않음
	if (OtherActor == OwnerBoss)
	{
		return;
	}

	// Player Tag 검사
	if (!OtherActor->ActorHasTag(TEXT("Player")))
	{
		return;
	}

	// 중복 Hit 검사
	if (HitActors.Contains(OtherActor))
	{
		return;
	}

	// Boss 검사
	if (!OwnerBoss)
	{
		return;
	}

	UBossCombatComponent* CombatComponent = OwnerBoss->GetCombatComponent();

	if (!CombatComponent)
	{
		return;
	}

	// 현재 공격 데이터
	const FBossAttackData* AttackData = CombatComponent->GetCurrentAttackData();

	if (!AttackData)
	{
		return;
	}

	// Hit 등록
	// 실제 Damage 처리 직전에 등록
	HitActors.Add(OtherActor);

	// Damage
	UGameplayStatics::ApplyDamage(OtherActor, AttackData->Damage, OwnerBoss->GetController(), OwnerBoss, UDamageType::StaticClass());

	UE_LOG(LogTemp, Log, TEXT("Boss Sword Hit: %s / Damage: %.1f"), *OtherActor->GetName(), AttackData->Damage);
}

void ABossGreatSword::EnableAttackCollision()
{
	// 새로운 공격 판정이 시작됐으므로 이전 공격에서 맞았던 Actor 기록 제거
	HitActors.Reset();

	HitCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	UE_LOG(LogTemp, Log, TEXT("Sword Collision Enabled"));
}

void ABossGreatSword::DisableAttackCollision()
{
	HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UE_LOG(LogTemp, Log, TEXT("Sword Collision Disabled"));
}
