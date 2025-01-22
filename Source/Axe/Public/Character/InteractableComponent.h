// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interact/InteractionOption.h"
#include "InteractableComponent.generated.h"


class UWidgetComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AXE_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractableComponent();
	FInteractionOption GetInteractionOptions();

	UFUNCTION(BlueprintCallable)
	bool InitInteractText();

	UFUNCTION(BlueprintCallable)
	void SetInteractTextVisibility(bool bVisible);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FInteractionOption InteractionOption;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UWidgetComponent* InteractionWidgetComponent;

private:
	bool bInitInteractText;
};
