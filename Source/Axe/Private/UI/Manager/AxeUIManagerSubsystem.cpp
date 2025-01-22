// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Manager/AxeUIManagerSubsystem.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/StackUserWidget.h"

bool UAxeUIManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

void UAxeUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UAxeUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UAxeUIManagerSubsystem::AddUIWidget(UStackUserWidget* Widget)
{
	if (!Widget)
	{
		return;
	}

	if (Widget->GetIsSole())
	{
		TArray<UStackUserWidget*> UIStackCopy = UIStack;
		for (UStackUserWidget* UserWidget : UIStackCopy)
		{
			RemoveUIWidget(UserWidget);
		}
	}
	else if (Widget->GetIsSingle())
	{
		TArray<UStackUserWidget*> SingleWidgets;
		GetWidgetInUIStackByClass(SingleWidgets, Widget->GetClass());
		for (UStackUserWidget* SingleWidget : SingleWidgets)
		{
			RemoveUIWidget(SingleWidget);
		}
	}

	UIStack.Push(Widget);
	Widget->AddToViewport();

	if (Widget->GetShowMouseCurse())
	{
		SetEnterUIInputMode(true);
	}
}

void UAxeUIManagerSubsystem::RemoveUIWidget(UStackUserWidget* Widget)
{
	if (!Widget)
	{
		return;
	}
	UIStack.Remove(Widget);
	Widget->RemoveFromParent();

	UStackUserWidget* TopUIWidget = GetTopUIWidget();
	if (TopUIWidget && TopUIWidget->GetShowMouseCurse())
	{
		SetEnterUIInputMode(true);
	}
	else
	{
		SetEnterUIInputMode(false);
	}
}

UStackUserWidget* UAxeUIManagerSubsystem::PopUIWidget()
{
	if (UIStack.Num() > 0)
	{
		UStackUserWidget* TopWidget = UIStack.Pop();
		if (TopWidget)
		{
			TopWidget->RemoveFromParent();
		}
		return TopWidget;
	}
	return nullptr;
}

UStackUserWidget* UAxeUIManagerSubsystem::GetTopUIWidget() const
{
	if (UIStack.Num() > 0)
	{
		return UIStack.Top();
	}
	return nullptr;
}

//
APlayerController* UAxeUIManagerSubsystem::GetPlayerController()
{
	if (!PlayerController)
	{
		PlayerController = GetWorld()->GetFirstPlayerController();
	}
	return PlayerController;
}

void UAxeUIManagerSubsystem::GetWidgetInUIStackByClass(TArray<UStackUserWidget*>& OutWidgets,
                                                       TSubclassOf<UStackUserWidget> WidgetClass) const
{
	for (UStackUserWidget* UserWidget : UIStack)
	{
		if (UserWidget->GetClass() == WidgetClass)
		{
			OutWidgets.Add(UserWidget);
		}
	}
}

void UAxeUIManagerSubsystem::SetEnterUIInputMode(bool bValue)
{
	UStackUserWidget* TopUIWidget = GetTopUIWidget();

	APlayerController* PC = GetPlayerController();
	if (bValue)
	{
		PC->SetShowMouseCursor(true);
		FInputModeUIOnly InputModeUIOnly = FInputModeUIOnly();
		PC->SetInputMode(InputModeUIOnly);

		PC->DisableInput(PC);
	}
	else
	{
		PC->SetShowMouseCursor(false);
		FInputModeGameOnly InputModeGameOnly = FInputModeGameOnly();
		PC->SetInputMode(InputModeGameOnly);

		PC->EnableInput(PC);
	}
}
