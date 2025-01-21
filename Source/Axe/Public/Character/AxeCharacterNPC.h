// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AxeCharacterMob.h"
#include "AxeCharacterNPC.generated.h"

/**
 * 
 */
UCLASS()
class AXE_API AAxeCharacterNPC : public AAxeCharacterMob
{
	GENERATED_BODY()
public:
	AAxeCharacterNPC();

protected:
	virtual void BeginPlay() override;
};
