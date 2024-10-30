// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/AbilityTask_HitTrace.h"

#include "Character/AxeCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"

UAbilityTask_HitTrace* UAbilityTask_HitTrace::CreateHitTraceTask(UGameplayAbility* OwningAbility,
                                                                 AAxeCharacterBase* Character,
                                                                 UStaticMeshComponent* TraceMeshComponent,
                                                                 FName BeginSocketName,
                                                                 FName EndSocketName,
                                                                 float Radius,
                                                                 TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
                                                                 bool bIgnoreSelf,
                                                                 TArray<AActor*>& IgnoreActors
)
{
	UAbilityTask_HitTrace* MyObj = NewAbilityTask<UAbilityTask_HitTrace>(OwningAbility);

	MyObj->Character = Character;
	MyObj->TraceMeshComponent = TraceMeshComponent;
	MyObj->BeginSocketName = BeginSocketName;
	MyObj->EndSocketName = EndSocketName;
	MyObj->Radius = Radius;
	MyObj->HitObjectTypes = ObjectTypes;
	MyObj->bIgnoreSelf = bIgnoreSelf;
	MyObj->IgnoreActors = IgnoreActors;

	return MyObj;
}

void UAbilityTask_HitTrace::Activate()
{
	Super::Activate();

	bTickingTask = true;
}

void UAbilityTask_HitTrace::OnDestroy(bool AbilityEnded)
{
	bTickingTask = false;
	Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_HitTrace::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	BeginSocketLocation = TraceMeshComponent->GetSocketLocation(BeginSocketName);
	EndSocketLocation = TraceMeshComponent->GetSocketLocation(EndSocketName);

	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::SphereTraceMultiForObjects(
		Character,
		BeginSocketLocation,
		EndSocketLocation,
		Radius,
		HitObjectTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::ForDuration,
		HitResults,
		bIgnoreSelf,
		FLinearColor::Red,
		FLinearColor::Green,
		3.f
	);
	HitTraceDelegate.Broadcast(HitResults);
}
