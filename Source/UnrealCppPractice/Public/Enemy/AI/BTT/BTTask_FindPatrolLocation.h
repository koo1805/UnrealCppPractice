// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolLocation.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCPPPRACTICE_API UBTTask_FindPatrolLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FindPatrolLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	// 현재 위치를 기준으로 랜덤 목적지를 찾을 반경
	UPROPERTY(EditAnywhere, Category = "Patrol")
	float PatrolRadius = 1000.0f;
};
