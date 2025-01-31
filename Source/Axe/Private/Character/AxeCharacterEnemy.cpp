// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AxeCharacterEnemy.h"

#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/AxeAttributeSet.h"
#include "Character/AxeCharacterPlayer.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widget/AxeUserWidget.h"
#include "UI/Widget/EnemyInfoBarWidgetComponent.h"
#include "UI/WidgetController/AxeWidgetControllerBase.h"
#include "UI/WidgetController/MobOverlayWidgetController.h"

AAxeCharacterEnemy::AAxeCharacterEnemy()
{
	// HealthBar
	EnemyInfoBar = CreateDefaultSubobject<UEnemyInfoBarWidgetComponent>("HealthBar");
	EnemyInfoBar->SetupAttachment(GetRootComponent());
	EnemyInfoBar->SetVisibility(false);
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

	const UWorld* World = GetWorld();
	// fixme 优化 GetAllPlayers
	TArray<AActor*> AllPlayers;
	UGameplayStatics::GetAllActorsOfClass(World, AAxeCharacterPlayer::StaticClass(), AllPlayers);
	SendLootToPlayers(AllPlayers);
}

void AAxeCharacterEnemy::BeginPlay()
{
	Super::BeginPlay();
	// HealthBar
	if (UAxeUserWidget* AxeUserWidget = Cast<UAxeUserWidget>(EnemyInfoBar->GetUserWidgetObject()))
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

	EnemyInfoBar->SetVisibility(false);
}

void AAxeCharacterEnemy::SendLootToPlayers(const TArray<AActor*>& Players)
{
	for (const AActor* OutPlayer : Players)
	{
		const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OutPlayer);
		if (!AbilitySystemInterface)
		{
			continue;
		}
		UAbilitySystemComponent* ASC = AbilitySystemInterface->GetAbilitySystemComponent();
		UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);

		// Xp
		AxeASC->ApplyIncomingXpEffect(this, LootXp);
		// GoldCoin
		AxeASC->ApplyIncomingGoldCoinCountEffect(this, LootGoldCoinCount);
	}
}
