// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnItemInstanceInfoUpdated);

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

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void FinishItemInstanceCreated();
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void UpdateItemInstance();

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual const FString& GetItemDescription();
	virtual FString CreateItemDescription();

	UFUNCTION(BlueprintCallable, Category = "Item")
	int32 GetItemInstancePrice() const { return ItemInstancePrice; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetItemInstancePrice(int32 InPrice) { ItemInstancePrice = InPrice; }

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual int32 GetItemInstanceDefaultPrice() const;
	virtual void InitItemInstancePrice();

	FOnItemInstanceInfoUpdated OnItemInstanceInfoUpdatedDelegate;

protected:
	UFUNCTION(BlueprintCallable)
	UAbilitySystemComponent* GetOwnerAbilitySystemComponent() const;

	UPROPERTY(Replicated)
	FString ItemInstanceDescription;

	UPROPERTY(Replicated)
	int32 ItemInstancePrice = -1;

private:
	// The item definition
	UPROPERTY(Replicated)
	TSubclassOf<UItemDefinition> ItemDef;

	UPROPERTY(Replicated)
	TObjectPtr<APawn> OwningPawn = nullptr;
};
