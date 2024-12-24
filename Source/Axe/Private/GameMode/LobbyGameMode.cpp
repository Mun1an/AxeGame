// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyGameMode.h"


void ALobbyGameMode::OnGameStart()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}
	const FString URL = FString::Printf(TEXT("%s?listen"), *ToMapURL);

	bUseSeamlessTravel = true;
	World->ServerTravel(URL);
}
