// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/AbilityTask_HitTrace.h"

#include "Engine/StaticMeshSocket.h"

UAbilityTask_HitTrace* UAbilityTask_HitTrace::CreateHitTraceTask(UGameplayAbility* OwningAbility,
                                                                 AAxeCharacterBase* Character,
                                                                 UStaticMeshComponent* TraceMeshComponent,
                                                                 FName BeginSocketName, FName EndSocketName,
                                                                 float Radius, bool bIgnoreSelf)
{
	UAbilityTask_HitTrace* MyObj = NewAbilityTask<UAbilityTask_HitTrace>(OwningAbility);

	MyObj->Character = Character;
	MyObj->TraceMeshComponent = TraceMeshComponent;
	MyObj->BeginSocketName = BeginSocketName;
	MyObj->EndSocketName = EndSocketName;
	MyObj->Radius = Radius;
	MyObj->bIgnoreSelf = bIgnoreSelf;

	return MyObj;
}

void UAbilityTask_HitTrace::Activate()
{
	Super::Activate();

	bTickingTask = true;
}

void UAbilityTask_HitTrace::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_HitTrace::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("HitTrace Task Tick"));

	BeginSocketLocation = TraceMeshComponent->GetSocketLocation(BeginSocketName);
	EndSocketLocation = TraceMeshComponent->GetSocketLocation(EndSocketName);

	DrawDebugLine(
		GetWorld(), BeginSocketLocation, EndSocketLocation,
		FColor::Red, false, 0.3f, 0, 10.f
	);
}
