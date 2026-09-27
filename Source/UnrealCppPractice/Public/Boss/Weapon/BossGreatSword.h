// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossGreatSword.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;
class ABossCharacter;

UCLASS()
class UNREALCPPPRACTICE_API ABossGreatSword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossGreatSword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnWeaponOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

public:
	// 공격 판정을 시작
	void EnableAttackCollision();

	// 공격 판정을 종료
	void DisableAttackCollision();

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UStaticMeshComponent> SwordMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UBoxComponent> HitCollision;

	// 이 무기를 사용하는 Boss
	UPROPERTY()
	TObjectPtr<ABossCharacter> OwnerBoss;

	// 현재 한 번의 공격 판정에서 이미 데미지를 받은 Actor 목록
	UPROPERTY()
	TSet<TObjectPtr<AActor>> HitActors;
};
