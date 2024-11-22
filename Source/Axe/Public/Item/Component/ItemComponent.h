// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemComponent.generated.h"


class UItemInstance;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AXE_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UItemComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item")
	FName ItemID;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item")
	int32 StackSize = 1;

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	TObjectPtr<UItemInstance> ItemInstance;
};
