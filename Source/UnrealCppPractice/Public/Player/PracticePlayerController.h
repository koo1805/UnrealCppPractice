// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PracticePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCPPPRACTICE_API APracticePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APracticePlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void Move(const struct FInputActionValue& InputValue);
	void Look(const struct FInputActionValue& InputValue);
	void JumpStart(const struct FInputActionValue& InputValue);
	void JumpEnd(const struct FInputActionValue& InputValue);

private:
	UPROPERTY()
	class ACharacter* PlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_PracticeMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Jump;
};
