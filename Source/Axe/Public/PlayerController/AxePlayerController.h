// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AxePlayerController.generated.h"

struct FGameplayTag;
class UAxeInputDataAsset;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class AXE_API AAxePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAxePlayerController();

	// 可获取到屏蔽输入后的移动向量
	FVector GetAxeLastMovementInputVector() const { return AxeLastMovementInputDirection; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;

	//
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void StopJumping(const FInputActionValue& Value);

	UPROPERTY(BlueprintReadOnly)
	FVector AxeLastMovementInputDirection;
	float ResetLastMovementInputMaxTime = 1.f;
	float ResetLastMovementInputTime = 0.f;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AxeInputMappingContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UAxeInputDataAsset> AbilityInputDataAsset;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
};
