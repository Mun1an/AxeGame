// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Tasks/AbilityTask_HitTrace.h"

#include "AbilitySystemComponent.h"
#include "Character/AxeCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"

UAbilityTask_HitTrace* UAbilityTask_HitTrace::CreateHitTraceTask(UGameplayAbility* OwningAbility,
                                                                 AAxeCharacterBase* Character,
                                                                 UMeshComponent* TraceMeshComponent,
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

	if (IsLocallyControlled())
	{
		// SendTargetDataToServer_Client();
	}
	else
	{
		// FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
		// Server
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		FAbilityTargetDataSetDelegate& AbilityTargetDataSetDelegate = AbilitySystemComponent.Get()->
			AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey);

		AbilityTargetDataSetDelegate.AddUObject(this, &UAbilityTask_HitTrace::OnHitDataReplicatedCallback_Server);

		bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(
			SpecHandle, ActivationPredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UAbilityTask_HitTrace::OnDestroy(bool AbilityEnded)
{
	bTickingTask = false;
	Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_HitTrace::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (IsLocallyControlled())
	{
		// Client
		SendTargetDataToServer_Client();
	}
	else
	{
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
	const FHitResult& FirstHitResult = HitResultList[0];
	if (HasHitActors.Contains(FirstHitResult.GetActor()))
	{
		return;
	}
	HasHitActors.Add(FirstHitResult.GetActor());

	// Window
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit(FirstHitResult);
	DataHandle.Add(Data);

	const FGameplayAbilitySpecHandle GameplayAbilitySpecHandle = GetAbilitySpecHandle();
	const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
	FPredictionKey ScopedPredictionKey = AbilitySystemComponent->ScopedPredictionKey;
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GameplayAbilitySpecHandle,
		ActivationPredictionKey,
		DataHandle,
		FGameplayTag(),
		ScopedPredictionKey
	);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		HitTraceDelegate.Broadcast(FirstHitResult);
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

		if (GameplayAbilityTargetData && GameplayAbilityTargetData->HasHitResult())
		{
			const FHitResult* HitResult = GameplayAbilityTargetData->GetHitResult();
			HitTraceDelegate.Broadcast(*HitResult);
		}
	}
}
