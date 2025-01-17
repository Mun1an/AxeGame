// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemComponent.generated.h"

class UItemDefinition;
class UItemInstance;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AXE_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UItemComponent();
	//
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;
	//~End of UObject interface
	
	TSubclassOf<UItemDefinition> GetItemDef() const { return ItemDef; }

	int32 GetStackCount() const { return StackCount; }
	void SetStackCount(int32 InStackCount) { StackCount = InStackCount; }

	UItemInstance* GetItemInstance() const { return ItemInstance; }
	UFUNCTION(BlueprintCallable)
	void SetItemInstance(UItemInstance* InItemInstance);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(Replicated, EditInstanceOnly, BlueprintReadOnly, Category = "Item")
	TSubclassOf<UItemDefinition> ItemDef;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Item")
	UItemInstance* ItemInstance = nullptr;

	UPROPERTY(Replicated, EditInstanceOnly, BlueprintReadOnly, Category = "Item")
	int32 StackCount = 1;

private:
};
