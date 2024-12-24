// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/LaunchCharacterNotifyState.h"

#include "Character/AxeCharacterPlayer.h"

ULaunchCharacterNotifyState::ULaunchCharacterNotifyState(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Cyan;
#endif
}
