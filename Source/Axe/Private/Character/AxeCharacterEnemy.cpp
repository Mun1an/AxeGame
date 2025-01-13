// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AxeCharacterEnemy.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"
#include "Character/AxeCharacterPlayer.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widget/AxeUserWidget.h"
#include "UI/WidgetController/AxeWidgetControllerBase.h"
#include "UI/WidgetController/MobOverlayWidgetController.h"

AAxeCharacterEnemy::AAxeCharacterEnemy()
{
	// HealthBar
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAxeCharacterEnemy::SetCombatTarget(AActor* NewCombatTarget)
{
	CombatTarget = NewCombatTarget;
}

AActor* AAxeCharacterEnemy::GetCombatTarget() const
{
	return CombatTarget;
}

void AAxeCharacterEnemy::OnDead()
{
	Super::OnDead();

	LootXpToAllPlayers();
}

void AAxeCharacterEnemy::BeginPlay()
{
	Super::BeginPlay();
	// HealthBar
	if (UAxeUserWidget* AxeUserWidget = Cast<UAxeUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		if (AbilitySystemComponent && AttributeSet)
		{
			const FWidgetControllerParams WidgetControllerParams(
				nullptr, nullptr,
				Cast<UAxeAbilitySystemComponent>(AbilitySystemComponent),
				Cast<UAxeAttributeSet>(AttributeSet)
			);
			UMobOverlayWidgetController* WidgetController = GetMobOverlayWidgetController(WidgetControllerParams);
			AxeUserWidget->SetWidgetController(WidgetController);

			WidgetController->BroadcastInitialValues();
		}
	}
}

UMobOverlayWidgetController* AAxeCharacterEnemy::GetMobOverlayWidgetController(const FWidgetControllerParams& Params)
{
	if (MobOverlayWidgetController == nullptr && MobOverlayWidgetControllerClass)
	{
		MobOverlayWidgetController = NewObject<UMobOverlayWidgetController>(
			this, MobOverlayWidgetControllerClass);
		MobOverlayWidgetController->SetWidgetControllerParams(Params);
		MobOverlayWidgetController->BindCallbacksToDependencies();
	}
	return MobOverlayWidgetController;
}

void AAxeCharacterEnemy::MulticastDeath_Implementation(FVector InDeathImpulseVector)
{
	Super::MulticastDeath_Implementation(InDeathImpulseVector);

	HealthBar->SetVisibility(false);
}

void AAxeCharacterEnemy::LootXpToAllPlayers()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	// fixme
	TArray<AActor*> OutPlayers;
	UGameplayStatics::GetAllActorsOfClass(World, AAxeCharacterPlayer::StaticClass(), OutPlayers);

	for (const AActor* OutPlayer : OutPlayers)
	{
		const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OutPlayer);
		if (!AbilitySystemInterface)
		{
			continue;
		}
		UAbilitySystemComponent* ASC = AbilitySystemInterface->GetAbilitySystemComponent();
		UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);
		AxeASC->ApplyIncomingXpEffect(this, LootXp);
	}
}
