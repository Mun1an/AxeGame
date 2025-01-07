// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/DisplayItemActor/DisplayItemActor.h"
#include "DisplayItemActor_Modular.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class AXE_API ADisplayItemActor_Modular : public ADisplayItemActor
{
	GENERATED_BODY()

public:
	ADisplayItemActor_Modular(const FObjectInitializer& ObjectInitializer);
	
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void OnAttachToParentActor(AActor* ParentActor) override;

	UFUNCTION(BlueprintCallable)
	void SetModularMeshByParentActor(AActor* ParentActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool InitModularSMList();
	//
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	
protected:
	
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	bool AddSMCompToModularSMList(USkeletalMeshComponent* SMComp);
	
	UPROPERTY(BlueprintReadWrite)
	TArray<USkeletalMeshComponent*> ModularSMList;
};
