// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AxeHUD.h"

#include "Blueprint/UserWidget.h"
#include "PlayerState/AxePlayerState.h"
#include "UI/Widget/AxeUserWidget.h"
#include "UI/WidgetController/AxeWidgetControllerBase.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void AAxeHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass is not set"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass is not set"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAxeUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();

	Widget->AddToViewport();
}

FWidgetControllerParams AAxeHUD::GetWidgetControllerParams() const
{
	FWidgetControllerParams Params;
	APlayerController* PlayerController = GetOwningPlayerController();
	AAxePlayerState* AxePlayerState = PlayerController->GetPlayerState<AAxePlayerState>();
	Params.PlayerController = PlayerController;
	Params.PlayerState = AxePlayerState;
	Params.AbilitySystemComponent = AxePlayerState->GetAbilitySystemComponent();
	Params.AttributeSet = AxePlayerState->GetAttributeSet();
	return Params;
}

UOverlayWidgetController* AAxeHUD::GetOverlayWidgetController()
{
	const FWidgetControllerParams Params = GetWidgetControllerParams();
	return GetOverlayWidgetController(Params);
}

UOverlayWidgetController* AAxeHUD::GetOverlayWidgetController(const FWidgetControllerParams& Params)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(Params);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}


void AAxeHUD::BeginPlay()
{
	Super::BeginPlay();
}
