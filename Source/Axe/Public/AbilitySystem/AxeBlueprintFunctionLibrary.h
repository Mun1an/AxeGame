// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AxeBlueprintFunctionLibrary.generated.h"

struct FGameplayAttribute;
class AAxeCharacterBase;
class UGameplayEffect;
class AAxeCharacterPlayer;
struct FGameplayEffectContext;
struct FGameplayEffectContextHandle;
struct FAxeGameplayEffectContext;
/**
 * 
 */
UCLASS()
class AXE_API UAxeBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="AxeBPLibrary|GameplayEffects")
	static FAxeGameplayEffectContext& GetAxeGameplayEffectContext(FGameplayEffectContextHandle& EffectContextHandle);
	static FAxeGameplayEffectContext* GetAxeGameplayEffectContext(FGameplayEffectContext* EffectContext);
	static const FAxeGameplayEffectContext* GetAxeGameplayEffectContext(const FGameplayEffectContext* EffectContext);

	UFUNCTION(BlueprintCallable, Category="AxeBPLibrary|Input")
	static FVector GetAxeLastMovementInputVector(const AAxeCharacterPlayer* AxeCharacterPlayer);

	UFUNCTION(BlueprintCallable, Category="AxeBPLibrary|Input")
	static FVector GetBottomGroundLocation(const AActor* SourceActor, FVector StartLocation,
	                                       const float LineLength = 1000);

	UFUNCTION(BlueprintPure, Category="AxeBPLibrary|Family")
	static bool IsFriend(const AActor* ActorA, const AActor* ActorB);

	UFUNCTION(BlueprintCallable, Category="AxeBPLibrary|GameplayEffect Attribute")
	static bool CanApplyAttributeModifiers(const UGameplayEffect* GameplayEffect, float Level,
									const FGameplayEffectContextHandle& EffectContext,
									const AAxeCharacterBase* ToTarget,
									TArray<FGameplayAttribute>& NotApplyAttributes);
};
