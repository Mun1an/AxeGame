// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Anim/AN/AxeAnimNotifyBase.h"
#include "CustomNameAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API UCustomNameAnimNotify : public UAxeAnimNotifyBase
{
	GENERATED_BODY()

public:
	UCustomNameAnimNotify(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName CustomName;
};
