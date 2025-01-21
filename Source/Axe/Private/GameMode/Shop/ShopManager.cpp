// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/Shop/ShopManager.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AShopManager::AShopManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;
}

void AShopManager::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AShopManager, ItemCount);
}

void AShopManager::BeginPlay()
{
	Super::BeginPlay();
}

