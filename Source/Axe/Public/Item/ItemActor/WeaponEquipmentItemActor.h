// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentItemActor.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "WeaponEquipmentItemActor.generated.h"


UCLASS(Blueprintable, Abstract)
class AXE_API AWeaponEquipmentItemActor : public AEquipmentItemActor
{
	GENERATED_BODY()

public:
	AWeaponEquipmentItemActor();

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }

	UFUNCTION(BlueprintCallable)
	virtual UMeshComponent* GetHitTraceMeshComponent() const;

	UFUNCTION(BlueprintCallable)
	UNiagaraComponent* GetWeaponTrailNiagaraComponent() const { return WeaponTrailNiagaraComponent; }

	UFUNCTION(BlueprintCallable)
	void SetWeaponTrail(bool bEnable, bool IsOverride = false);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetWeaponTrail(bool bEnable);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	USceneComponent* SceneComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
	UNiagaraComponent* WeaponTrailNiagaraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponTrail")
	FName WeaponTrailAttachSocketName = FName("WeaponTrail");

	UPROPERTY(EditDefaultsOnly, Category = "WeaponTrail")
	FName WeaponTrailNiagaraWidthVariableName = FName("Particles.RibbonWidth");

	UPROPERTY(EditDefaultsOnly, Category = "WeaponTrail")
	float WeaponTrailWidth = 50.f;

	int32 WeaponTrailEnableCount = 0;
};
