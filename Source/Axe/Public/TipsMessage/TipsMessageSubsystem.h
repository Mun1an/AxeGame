// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TipsMessageSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSendTipsMessageSignature, const FString&, TipsMessage);
/**
 * 
 */
UCLASS()
class AXE_API UTipsMessageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UTipsMessageSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable)
	void SendTipsMessage(const FString& Message);

	FOnSendTipsMessageSignature OnSendTipsMessageSignature;
};
