// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Animation/NotifyState/AnimNotifyState_BossWeaponTrace.h"
#include <Boss/BossCharacter.h>
#include <Boss/Weapon/BossGreatSword.h>
#include <Components/SkeletalMeshComponent.h>

void UAnimNotifyState_BossWeaponTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp)
	{
		return;
	}

	ABossCharacter* Boss = Cast<ABossCharacter>(MeshComp->GetOwner());

	if (!Boss)
	{
		return;
	}

	ABossGreatSword* Sword = Boss->GetGreatSword();

	if (!Sword)
	{
		return;
	}

	Sword->EnableAttackCollision();
}

void UAnimNotifyState_BossWeaponTrace::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference & EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}

	ABossCharacter* Boss = Cast<ABossCharacter>(MeshComp->GetOwner());

	if (!Boss)
	{
		return;
	}

	ABossGreatSword* Sword = Boss->GetGreatSword();

	if (!Sword)
	{
		return;
	}

	Sword->DisableAttackCollision();
}
