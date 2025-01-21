// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShopManager.generated.h"

// UCLASS(notplaceable, Transient)
UCLASS(notplaceable)
class AXE_API AShopManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShopManager();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadWrite)
	float ItemCount = 1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
};
