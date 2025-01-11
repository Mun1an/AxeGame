// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInstance.generated.h"

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

	APawn* GetPawn() const { return OwningPawn; }
	void SetPawn(APawn* InPawn) { OwningPawn = InPawn; }

	const TSubclassOf<UItemDefinition>& GetItemDef()
	{
		return ItemDef;
	}

	void SetItemDef(TSubclassOf<UItemDefinition> InDef);

	virtual void OnItemInstanceCreated();

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual FString GetItemDescription();
	
protected:
	// The item definition
	UPROPERTY(Replicated, BlueprintReadOnly)
	TSubclassOf<UItemDefinition> ItemDef;

	UPROPERTY(Replicated, BlueprintReadOnly)
	TObjectPtr<APawn> OwningPawn = nullptr;
};
