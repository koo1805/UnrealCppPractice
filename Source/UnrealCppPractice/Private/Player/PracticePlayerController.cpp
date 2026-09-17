// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PracticePlayerController.h"
#include <GameFramework/Character.h>
#include <InputMappingContext.h>
#include <InputAction.h>
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>

APracticePlayerController::APracticePlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> imc_PracticeMappingContext(TEXT("/Game/Input/IMC_PracticeMappingContext.IMC_PracticeMappingContext"));
	if (imc_PracticeMappingContext.Succeeded())
	{
		IMC_PracticeMappingContext = imc_PracticeMappingContext.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ia_Move(TEXT("/Game/Input/Action/IA_Move.IA_Move"));
	if (ia_Move.Succeeded())
	{
		IA_Move = ia_Move.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ia_Look(TEXT("/Game/Input/Action/IA_Look.IA_Look"));
	if (ia_Look.Succeeded())
	{
		IA_Look = ia_Look.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ia_Jump(TEXT("/Game/Input/Action/IA_Jump.IA_Jump"));
	if (ia_Jump.Succeeded())
	{
		IA_Jump = ia_Jump.Object;
	}
}

void APracticePlayerController::BeginPlay()
{
	PlayerCharacter = GetPawn<ACharacter>();

	auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (subsystem)
	{
		subsystem->AddMappingContext(IMC_PracticeMappingContext, 0);
	}
}

void APracticePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(InputComponent);

	// Bind
	EnhancedInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APracticePlayerController::Move);
	EnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APracticePlayerController::Look);
	EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Started, this, &APracticePlayerController::JumpStart);
	EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Completed, this, &APracticePlayerController::JumpEnd);
}

void APracticePlayerController::Move(const FInputActionValue & InputValue)
{
	const FVector2D MovementVector = InputValue.Get<FVector2D>();

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return;
	}

	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
	ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
}

void APracticePlayerController::Look(const FInputActionValue & InputValue)
{
	const FVector2D LookAxisVector = InputValue.Get<FVector2D>();

	const float Turn = LookAxisVector.X;
	const float LookUp = LookAxisVector.Y;

	AddYawInput(Turn);
	AddPitchInput(LookUp);
}

void APracticePlayerController::JumpStart(const FInputActionValue & InputValue)
{
	if (!IsValid(PlayerCharacter))
	{
		return;
	}

	PlayerCharacter->Jump();
}

void APracticePlayerController::JumpEnd(const FInputActionValue & InputValue)
{
	if (!IsValid(PlayerCharacter))
	{
		return;
	}

	PlayerCharacter->StopJumping();
}