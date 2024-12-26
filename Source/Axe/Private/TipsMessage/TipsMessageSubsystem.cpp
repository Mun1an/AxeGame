// Fill out your copyright notice in the Description page of Project Settings.


#include "TipsMessage/TipsMessageSubsystem.h"

UTipsMessageSubsystem::UTipsMessageSubsystem()
{
}

void UTipsMessageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UTipsMessageSubsystem::SendTipsMessage(const FString& Message)
{
	OnSendTipsMessageSignature.Broadcast(Message);
}
