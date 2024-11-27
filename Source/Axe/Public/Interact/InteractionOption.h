// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractionOption.generated.h"

class UGameplayAbility;
class IInteractableInterface;
class IInteractableTarget;
class UUserWidget;

/**  */
USTRUCT(BlueprintType)
struct FInteractionOption
{
	GENERATED_BODY()

	static FInteractionOption Empty;

	/** The interactable target */
	UPROPERTY(BlueprintReadWrite)
	AActor* InteractableTarget = nullptr;

	/** Simple text the interaction might return */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;

	/** Simple sub-text the interaction might return */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SubText;

	/** The ability to grant the avatar when they get near interactable objects. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> InteractionAbilityToGrant;
};
