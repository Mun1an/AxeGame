// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile/AxeProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "Axe/Axe.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AAxeProjectile::AAxeProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovementComponent->InitialSpeed = 500.f;
	ProjectileMovementComponent->MaxSpeed = 500.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.01f;
}

AAxeProjectile* AAxeProjectile::SpawnProjectile(TSubclassOf<AAxeProjectile>& ProjectileClass, AActor* SourceActor,
                                                const FDamageEffectParams& DamageParam,
                                                const FTransform& SpawnTransform)
{
	check(SourceActor);
	UWorld* World = GEngine->GetWorldFromContextObject(SourceActor, EGetWorldErrorMode::LogAndReturnNull);
	AAxeProjectile* Projectile = World->SpawnActorDeferred<AAxeProjectile>(
		ProjectileClass,
		SpawnTransform,
		SourceActor,
		Cast<APawn>(SourceActor),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	Projectile->DamageEffectParams = DamageParam;
	Projectile->SourceActor = SourceActor;
	Projectile->FinishSpawning(SpawnTransform);
	return Projectile;
}

// Called when the game starts or when spawned
void AAxeProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAxeProjectile::OnOverlapOverlap);

	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

void AAxeProjectile::OnOverlapOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	if (SourceActor == OtherActor)
	{
		return;
	}
	if (bHit)
	{
		return;
	}
	bHit = true;

	if (HasAuthority())
	{
		UAbilitySystemComponent* OtherASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		if (OtherASC)
		{
			// Damage
			UAxeAbilitySystemComponent* OtherAxeASC = Cast<UAxeAbilitySystemComponent>(OtherASC);
			OtherAxeASC->ApplyDamageEffectToSelf(SourceActor, DamageEffectParams);
		}
		Destroy();
	}
}

void AAxeProjectile::Destroyed()
{
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	
	UGameplayStatics::PlaySoundAtLocation(
		this,
		ImpactSound,
		GetActorLocation(),
		FRotator::ZeroRotator
	);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		ImpactEffect,
		GetActorLocation()
	);

	Super::Destroyed();
}
