// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractionOption.generated.h"

class IInteractableInterface;
class IInteractableTarget;
class UUserWidget;

/**  */
USTRUCT(BlueprintType)
struct FInteractionOption
{
	GENERATED_BODY()

	/** The interactable target */
	UPROPERTY(BlueprintReadWrite)
	TScriptInterface<IInteractableInterface> InteractableTarget;

	/** Simple text the interaction might return */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;

	/** Simple sub-text the interaction might return */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SubText;

	// UI
	//--------------------------------------------------------------

	/** The widget to show for this kind of interaction. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<UUserWidget> InteractionWidgetClass;
};
