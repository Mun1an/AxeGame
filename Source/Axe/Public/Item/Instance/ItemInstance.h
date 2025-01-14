// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInstance.generated.h"

class UAbilitySystemComponent;
class UItemDefinition;
/**
 * 
 */
UCLASS(BlueprintType)
class AXE_API UItemInstance : public UObject
{
	GENERATED_BODY()

public:
	UItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }

	UFUNCTION(BlueprintCallable)
	APawn* GetPawn() const { return OwningPawn; }

	void SetPawn(APawn* InPawn) { OwningPawn = InPawn; }

	UFUNCTION(BlueprintCallable)
	TSubclassOf<UItemDefinition> GetItemDef()
	{
		return ItemDef;
	}

	void SetItemDef(TSubclassOf<UItemDefinition> InDef);

	virtual void OnItemInstanceCreated();

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual FString GetItemDescription();

protected:
	UFUNCTION(BlueprintCallable)
	UAbilitySystemComponent* GetOwnerAbilitySystemComponent() const;

private:
	// The item definition
	UPROPERTY(Replicated)
	TSubclassOf<UItemDefinition> ItemDef;

	UPROPERTY(Replicated)
	TObjectPtr<APawn> OwningPawn = nullptr;
};
