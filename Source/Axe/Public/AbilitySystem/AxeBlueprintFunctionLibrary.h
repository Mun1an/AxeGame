// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AxeBlueprintFunctionLibrary.generated.h"

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
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemBPLibrary|GameplayEffects")
	static FAxeGameplayEffectContext& GetAxeGameplayEffectContext(FGameplayEffectContextHandle& EffectContextHandle);
	static FAxeGameplayEffectContext* GetAxeGameplayEffectContext(FGameplayEffectContext* EffectContext);
	static const FAxeGameplayEffectContext* GetAxeGameplayEffectContext(const FGameplayEffectContext* EffectContext);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemBPLibrary|Input")
	static FVector GetAxeLastMovementInputVector(const AAxeCharacterPlayer* AxeCharacterPlayer);
};
