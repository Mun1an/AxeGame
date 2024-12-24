// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/ComboAnimNotifyState.h"


UComboAnimNotifyState::UComboAnimNotifyState(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Orange;
#endif
}
