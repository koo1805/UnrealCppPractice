#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"
#include "BossCombatTypes.generated.h"


// 보스가 사용할 수 있는 공격 종류
UENUM(BlueprintType)
enum class EBossAttackType : uint8
{
	None			UMETA(DisplayName = "None"),	
	Normal			UMETA(DisplayName = "Normal Attack"),			// 근거리 일반 공격
	Combo			UMETA(DisplayName = "Combo Attack"),			// 근거리 연속 공격
	DelayedHeavy	UMETA(DisplayName = "Delayed Heavy Attack"),	// 근거리 엇박 강공격
	Leap			UMETA(DisplayName = "Leap Attack"),				// 중거리 도약 공격
	ApproachHeavy	UMETA(DisplayName = "Approach Heavy Attack"),	// 장거리 접근 강공격
	SwordWave		UMETA(DisplayName = "Sword Wave")				// 장거리 검기 공격
};


// 공격 하나가 가지고 있는 데이터
USTRUCT(BlueprintType)
struct FBossAttackData
{
	GENERATED_BODY()

public:
	// 공격 종류
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	EBossAttackType AttackType = EBossAttackType::None;

	// 공격 한 번의 기본 데미지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float Damage = 10.0f;

	// 이 공격을 사용할 수 있는 최대 거리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float AttackRange = 200.0f;

	// 공격을 다시 사용할 수 있을 때까지의 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float Cooldown = 0.0f;

	// 공격 선택 시 사용될 가중치
	// 근접 공격 랜덤 선택 등에 사용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float Weight = 1.0f;

	// 공격에 사용할 애니메이션 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackMontage = nullptr;
};

// 거리 판단 데이터
USTRUCT(BlueprintType)
struct FBossDistanceData
{
	GENERATED_BODY()

public:
	// 이 거리 이하면 근거리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Distance")
	float CloseRange = 300.0f;

	// 이 거리 이하면 중거리
	// 이보다 멀면 장거리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Distance")
	float MidRange = 800.0f;
};