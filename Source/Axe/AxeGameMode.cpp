// Copyright Epic Games, Inc. All Rights Reserved.

#include "AxeGameMode.h"
#include "AxeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAxeGameMode::AAxeGameMode()
{
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	// if (PlayerPawnBPClass.Class != NULL)
	// {
	// 	DefaultPawnClass = PlayerPawnBPClass.Class;
	// }
}
