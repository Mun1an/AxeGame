// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AxeBlueprintFunctionLibrary.h"

#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "AssetManager/AxeAssetManager.h"
#include "AssetManager/AxeGameData.h"
#include "Character/AxeCharacterPlayer.h"
#include "Enum/AxeTypes.h"
#include "PlayerController/AxePlayerController.h"

FAxeGameplayEffectContext& UAxeBlueprintFunctionLibrary::GetAxeGameplayEffectContext(
	FGameplayEffectContextHandle& EffectContextHandle)
{
	FGameplayEffectContext* EffectContext = EffectContextHandle.Get();
	FAxeGameplayEffectContext* AxeEffectContext = static_cast<FAxeGameplayEffectContext*>(EffectContext);
	return *AxeEffectContext;
}

FAxeGameplayEffectContext* UAxeBlueprintFunctionLibrary::GetAxeGameplayEffectContext(
	FGameplayEffectContext* EffectContext)
{
	FAxeGameplayEffectContext* AxeEffectContext = static_cast<FAxeGameplayEffectContext*>(EffectContext);
	return AxeEffectContext;
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

bool UAxeBlueprintFunctionLibrary::IsFriend(const AActor* ActorA, const AActor* ActorB)
{
	const bool bIsPlayerA = ActorA->ActorHasTag(FName("Player"));
	const bool bIsPlayerB = ActorB->ActorHasTag(FName("Player"));
	const bool bIsEnemyA = ActorA->ActorHasTag(FName("Enemy"));
	const bool bIsEnemyB = ActorB->ActorHasTag(FName("Enemy"));

	return (bIsPlayerA && bIsPlayerB) || (bIsEnemyA && bIsEnemyB);
}

bool UAxeBlueprintFunctionLibrary::CanApplyAttributeModifiers(const UGameplayEffect* GameplayEffect, float Level,
                                                              const FGameplayEffectContextHandle& EffectContext,
                                                              const AAxeCharacterBase* ToTarget,
                                                              TArray<FGameplayAttribute>& NotApplyAttributes)
{
	FGameplayEffectSpec Spec(GameplayEffect, EffectContext, Level);

	Spec.CalculateModifierMagnitudes();

	UAttributeSet* AttributeSet = ToTarget->GetAttributeSet();

	for (int32 ModIdx = 0; ModIdx < Spec.Modifiers.Num(); ++ModIdx)
	{
		const FGameplayModifierInfo& ModDef = Spec.Def->Modifiers[ModIdx];
		const FModifierSpec& ModSpec = Spec.Modifiers[ModIdx];

		// It only makes sense to check additive operators
		if (ModDef.ModifierOp == EGameplayModOp::Additive)
		{
			if (!ModDef.Attribute.IsValid())
			{
				continue;
			}
			float CurrentValue = ModDef.Attribute.GetNumericValueChecked(AttributeSet);
			float CostValue = ModSpec.GetEvaluatedMagnitude();

			if (CurrentValue + CostValue < 0.f)
			{
				NotApplyAttributes.AddUnique(ModDef.Attribute);
			}
		}
	}
	if (NotApplyAttributes.Num() > 0)
	{
		return false;
	}
	return true;
}

bool UAxeBlueprintFunctionLibrary::SetHighLight(UMeshComponent* MeshComponent, bool bHighLight, int32 StencilValue)
{
	if (!MeshComponent)
	{
		return false;
	}
	MeshComponent->SetCustomDepthStencilValue(StencilValue);
	MeshComponent->SetRenderCustomDepth(bHighLight);
	return true;
}

bool UAxeBlueprintFunctionLibrary::SetHighLights(const TArray<UMeshComponent*>& MeshComponents, bool bHighLight,
                                                 int32 StencilValue)
{
	for (UMeshComponent* MeshComponent : MeshComponents)
	{
		if (!MeshComponent)
		{
			continue;
		}
		MeshComponent->SetCustomDepthStencilValue(StencilValue);
		MeshComponent->SetRenderCustomDepth(bHighLight);
	}
	return true;
}

APawn* UAxeBlueprintFunctionLibrary::GetLocalPawn(const UObject* WorldContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		return nullptr;
	}
	const APlayerController* FirstLocalPlayerController = GEngine->GetFirstLocalPlayerController(World);
	if (!FirstLocalPlayerController)
	{
		return nullptr;
	}
	APawn* LocalPawn = FirstLocalPlayerController->GetPawn();
	return LocalPawn;
}

const FAxeAttributeUIInfo& UAxeBlueprintFunctionLibrary::GetAttributeUIInfoByTag(FGameplayTag AttributeTag)
{
	UDataAsset* DataAsset = UAxeAssetManager::GetAsset(
		UAxeGameData::Get().DA_AttributeUIInfo
	);
	if (!DataAsset)
	{
		return FAxeAttributeUIInfo::Empty;
	}
	UAttributeUIDataAsset* AttributeUIDataAsset = Cast<UAttributeUIDataAsset>(DataAsset);

	const FAxeAttributeUIInfo& FindAttributeInfoForTag = AttributeUIDataAsset->FindAttributeInfoForTag(AttributeTag);
	return FindAttributeInfoForTag;
}
