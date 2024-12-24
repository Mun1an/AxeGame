// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AN/CustomNameAnimNotify.h"

UCustomNameAnimNotify::UCustomNameAnimNotify(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Cyan;
#endif
}