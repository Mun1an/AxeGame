// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enum/AxeTypes.h"
#include "GameFramework/Actor.h"
#include "AxeProjectile.generated.h"

class UProjectileMovementComponent;
class USoundBase;
class UNiagaraSystem;
class USphereComponent;
class UAudioComponent;

UCLASS()
class AXE_API AAxeProjectile : public AActor
{
	GENERATED_BODY()

public:
	AAxeProjectile();

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	AActor* SourceActor;

	static AAxeProjectile* SpawnProjectile(
		TSubclassOf<AAxeProjectile>& ProjectileClass,
		AActor* SourceActor, const FDamageEffectParams& DamageParam, const FTransform& SpawnTransform
	);

	//
	UPROPERTY(EditAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                      int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void Destroyed() override;

private:
	bool bHit;

	UPROPERTY(EditAnywhere)
	float LifeSpan = 5.0;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

};
