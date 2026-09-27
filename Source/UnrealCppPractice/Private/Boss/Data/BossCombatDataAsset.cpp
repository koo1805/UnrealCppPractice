// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Data/BossCombatDataAsset.h"

const FBossAttackData* UBossCombatDataAsset::FindAttackData(EBossAttackType AttackType) const
{
	for (const FBossAttackData& AttackData : Attacks)
	{
		if (AttackData.AttackType == AttackType)
		{
			return &AttackData;
		}
	}

	return nullptr;
}
