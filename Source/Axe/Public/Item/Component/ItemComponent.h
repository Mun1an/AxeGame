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

	TSubclassOf<UItemDefinition> GetItemDef() const { return ItemDef; }
	int32 GetStackCount() const { return StackCount; }
	
	UStaticMesh* GetStaticMeshInItemFragment_World();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item")
	TSubclassOf<UItemDefinition> ItemDef;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item")
	int32 StackCount = 1;
};
