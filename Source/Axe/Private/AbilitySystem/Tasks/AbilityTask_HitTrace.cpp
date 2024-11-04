// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/AbilityTask_HitTrace.h"

#include "AbilitySystemComponent.h"
#include "Character/AxeCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"

UAbilityTask_HitTrace* UAbilityTask_HitTrace::CreateHitTraceTask(UGameplayAbility* OwningAbility,
                                                                 AAxeCharacterBase* Character,
                                                                 UStaticMeshComponent* TraceMeshComponent,
                                                                 FName BeginSocketName,
                                                                 FName EndSocketName,
                                                                 float Radius,
                                                                 const TArray<TEnumAsByte<EObjectTypeQuery>>&
                                                                 ObjectTypes,
                                                                 bool bIgnoreSelf,
                                                                 const TArray<AActor*>& IgnoreActors
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

	bool bIsLocallyControlled = IsLocallyControlled();

	if (bIsLocallyControlled)
	{
		// Client
		SendTargetDataToServer_Client();
	}
	else
	{
		// Server
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(
			GetAbilitySpecHandle(),
			GetActivationPredictionKey()
		).AddUObject(this, &UAbilityTask_HitTrace::OnHitDataReplicatedCallback_Server);
	}
}

void UAbilityTask_HitTrace::SendTargetDataToServer_Client()
{
	BeginSocketLocation = TraceMeshComponent->GetSocketLocation(BeginSocketName);
	EndSocketLocation = TraceMeshComponent->GetSocketLocation(EndSocketName);
	TArray<FHitResult> HitResultList;
	UKismetSystemLibrary::SphereTraceMultiForObjects(
		Character,
		BeginSocketLocation,
		EndSocketLocation,
		Radius,
		HitObjectTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		HitResultList,
		bIgnoreSelf,
		FLinearColor::Red,
		FLinearColor::Green,
		3.f
	);
	if (HitResultList.Num() <= 0)
	{
		return;
	}

	// Window
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit(HitResultList[0]);
	DataHandle.Add(Data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey
	);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		HitTraceDelegate.Broadcast(HitResultList[0]);
	}
}

void UAbilityTask_HitTrace::OnHitDataReplicatedCallback_Server(const FGameplayAbilityTargetDataHandle& DataHandle,
                                                               FGameplayTag ActivationTag)
{
	// Consume
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		const FGameplayAbilityTargetData* GameplayAbilityTargetData = DataHandle.Get(0);
		const FHitResult* HitResult = GameplayAbilityTargetData->GetHitResult();
		HitTraceDelegate.Broadcast(*HitResult);
	}
}
