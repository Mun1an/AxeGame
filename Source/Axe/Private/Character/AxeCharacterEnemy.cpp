// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AxeCharacterEnemy.h"

#include "Components/WidgetComponent.h"
#include "UI/Widget/AxeUserWidget.h"
#include "UI/WidgetController/AxeWidgetControllerBase.h"
#include "UI/WidgetController/MobOverlayWidgetController.h"

AAxeCharacterEnemy::AAxeCharacterEnemy()
{
	// HealthBar
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
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
				nullptr, nullptr, AbilitySystemComponent, AttributeSet
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
		MobOverlayWidgetController = NewObject<UMobOverlayWidgetController>(this, MobOverlayWidgetControllerClass);
		MobOverlayWidgetController->SetWidgetControllerParams(Params);
		MobOverlayWidgetController->BindCallbacksToDependencies();
	}
	return MobOverlayWidgetController;
}
