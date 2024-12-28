// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ModularCharacterComponent.generated.h"

class AAxeCharacterPlayer;

UENUM(BlueprintType)
enum class EAxeModularCharacterSM : uint8
{
	Gender_Head,
	Gender_Eyebrows,
	Gender_FacialHair,
	Gender_Torso,
	Gender_ArmUpperArm_Right,
	Gender_ArmUpperArm_Left,
	Gender_ArmLowerArm_Right,
	Gender_HandRight,
	Gender_ArmLowerArm_Left,
	Gender_HandLeft,
	Gender_Hips,
	Gender_LegRight,
	Gender_LegLeft,
	All_HeadCoverings_NoHair,
	All_HeadCoverings_BaseHair,
	All_HeadCoverings_NoFacialHair,
	All_Hair,
	All_HeadAttachment,
	All_ChestAttachment,
	All_BackAttachment,
	All_ShoulderAttachment_Right,
	All_ShoulderAttachment_Left,
	All_ElbowAttachment_Right,
	All_ElbowAttachment_Left,
	All_HipsAttachment,
	All_KneeAttachment_Right,
	All_KneeAttachment_Left,
	All_Extra,
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AXE_API UModularCharacterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UModularCharacterComponent();

	UFUNCTION(BlueprintCallable)
	void SetModularMesh();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	AAxeCharacterPlayer* GetAxeCharacterPlayerOwner();

private:
	UPROPERTY()
	AAxeCharacterPlayer* AxeCharacterPlayerOwner = nullptr;
};
