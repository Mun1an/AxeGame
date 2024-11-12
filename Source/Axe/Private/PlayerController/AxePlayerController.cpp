// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/AxePlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "ActionSystem/ComboActionComponent.h"
#include "Character/AxeCharacterPlayer.h"
#include "Input/AxeEnhancedInputComponent.h"

AAxePlayerController::AAxePlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAxePlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AxeInputMappingContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()
	);
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AxeInputMappingContext, 0);
	}
}

void AAxePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 定时清除上次的自主移动输入
	if (ResetLastMovementInputTime > 0)
	{
		ResetLastMovementInputTime -= DeltaSeconds;
	}
	else if (AxeLastMovementInputDirection != FVector::ZeroVector)
	{
		AxeLastMovementInputDirection = FVector::ZeroVector;
	}
}

void AAxePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UAxeEnhancedInputComponent* EnhancedInputComponent = CastChecked<UAxeEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAxePlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAxePlayerController::Look);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AAxePlayerController::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AAxePlayerController::StopJumping);

	// Bind Ability Action
	if (AbilityInputDataAsset)
	{
		EnhancedInputComponent->BindAbilityActions(
			AbilityInputDataAsset, this,
			&ThisClass::AbilityInputTagPressed,
			&ThisClass::AbilityInputTagReleased,
			&ThisClass::AbilityInputTagHeld
		);
	}
}

void AAxePlayerController::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(GetCharacter());
	if (AxeCharacterPlayer)
	{
		AxeCharacterPlayer->AddMovementInput(ForwardDirection, MovementVector.Y);
		AxeCharacterPlayer->AddMovementInput(RightDirection, MovementVector.X);
	}

	ResetLastMovementInputTime = ResetLastMovementInputMaxTime;
	AxeLastMovementInputDirection = ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X;
}

void AAxePlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(GetCharacter());
	if (AxeCharacterPlayer)
	{
		AxeCharacterPlayer->AddControllerYawInput(LookAxisVector.X);
		AxeCharacterPlayer->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AAxePlayerController::Jump(const FInputActionValue& Value)
{
	AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(GetCharacter());
	if (AxeCharacterPlayer)
	{
		AxeCharacterPlayer->Jump();
	}
}

void AAxePlayerController::StopJumping(const FInputActionValue& Value)
{
	AAxeCharacterPlayer* AxeCharacterPlayer = Cast<AAxeCharacterPlayer>(GetCharacter());
	if (AxeCharacterPlayer)
	{
		AxeCharacterPlayer->StopJumping();
	}
}


void AAxePlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetCharacter()));
	if (AxeASC)
	{
		AxeASC->AbilityInputTagPressed(InputTag);
	}
}

void AAxePlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetCharacter()));
	if (AxeASC)
	{
		AxeASC->AbilityInputTagReleased(InputTag);
	}
}

void AAxePlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetCharacter()));
	if (AxeASC)
	{
		AxeASC->AbilityInputTagHeld(InputTag);
	}
}
