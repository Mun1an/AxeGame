// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/ComboInputCacheAnimNotifyState.h"

#include "ActionSystem/ComboActionComponent.h"


UComboInputCacheAnimNotifyState::UComboInputCacheAnimNotifyState(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Yellow;
#endif
}
