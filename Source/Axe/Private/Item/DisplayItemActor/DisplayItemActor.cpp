// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/DisplayItemActor/DisplayItemActor.h"

ADisplayItemActor::ADisplayItemActor(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer)
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	RootComponent = SceneComponent;
}

void ADisplayItemActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ADisplayItemActor::OnAttachToParentActor(AActor* ParentActor)
{
}

void ADisplayItemActor::HighlightActor()
{
	BP_HighlightActor();
}

void ADisplayItemActor::UnHighlightActor()
{
	BP_UnHighlightActor();
}
