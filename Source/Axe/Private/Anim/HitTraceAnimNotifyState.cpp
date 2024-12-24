// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/HitTraceAnimNotifyState.h"

UHitTraceAnimNotifyState::UHitTraceAnimNotifyState(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Red;
#endif
}
