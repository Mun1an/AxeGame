// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HighLight.h"
#include "DisplayItemActor.generated.h"


UCLASS(Blueprintable, Abstract)
class AXE_API ADisplayItemActor : public AActor, public IHighLight
{
	GENERATED_BODY()

public:
	ADisplayItemActor(const FObjectInitializer& ObjectInitializer);
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void OnAttachToParentActor(AActor* ParentActor);
	
	// HighLight
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_HighlightActor();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_UnHighlightActor();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	USceneComponent* SceneComponent;
};
