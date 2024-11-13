// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AxeBlueprintFunctionLibrary.h"

#include "GameplayEffectTypes.h"
#include "Character/AxeCharacterPlayer.h"
#include "Enum/AxeTypes.h"
#include "PlayerController/AxePlayerController.h"

FAxeGameplayEffectContext& UAxeBlueprintFunctionLibrary::GetAxeGameplayEffectContext(
	FGameplayEffectContextHandle& EffectContextHandle)
{
	FGameplayEffectContext* EffectContext = EffectContextHandle.Get();
	FAxeGameplayEffectContext* AuraEffectContext = static_cast<FAxeGameplayEffectContext*>(EffectContext);
	return *AuraEffectContext;
}

FAxeGameplayEffectContext* UAxeBlueprintFunctionLibrary::GetAxeGameplayEffectContext(
	FGameplayEffectContext* EffectContext)
{
	FAxeGameplayEffectContext* AuraEffectContext = static_cast<FAxeGameplayEffectContext*>(EffectContext);
	return AuraEffectContext;
}

const FAxeGameplayEffectContext* UAxeBlueprintFunctionLibrary::GetAxeGameplayEffectContext(
	const FGameplayEffectContext* EffectContext)
{
	const FAxeGameplayEffectContext* AxeGameplayEffectContext = static_cast<const FAxeGameplayEffectContext*>(
		EffectContext);
	return AxeGameplayEffectContext;
}

FVector UAxeBlueprintFunctionLibrary::GetAxeLastMovementInputVector(const AAxeCharacterPlayer* AxeCharacterPlayer)
{
	check(AxeCharacterPlayer)
	AController* Controller = AxeCharacterPlayer->GetController();
	if (Controller)
	{
		if (AAxePlayerController* AxePlayerController = Cast<AAxePlayerController>(Controller))
		{
			return AxePlayerController->GetAxeLastMovementInputVector();
		}
	}
	return AxeCharacterPlayer->GetLastMovementInputVector();
}

FVector UAxeBlueprintFunctionLibrary::GetBottomGroundLocation(const AActor* SourceActor, FVector StartLocation,
                                                              const float LineLength)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(SourceActor, EGetWorldErrorMode::LogAndReturnNull);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(SourceActor);
	FVector EndLocation = StartLocation - FVector(0, 0, LineLength);
	bool bHit = World->LineTraceSingleByChannel(
		HitResult, StartLocation,
		EndLocation,
		ECC_Visibility, CollisionParams
	);
	if (bHit)
	{
		return HitResult.ImpactPoint;
	}
	else
	{
		return EndLocation;
	}
}
