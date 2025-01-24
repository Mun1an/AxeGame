// wqz gas aura 


#include "Character/EffectNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Character/AxeCharacterBase.h"
#include "Interface/DeadInterface.h"

UEffectNiagaraComponent::UEffectNiagaraComponent()
{
	bWantsInitializeComponent = true;
	bAutoActivate = false;
}

void UEffectNiagaraComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UEffectNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	AAxeCharacterBase* AxeCharacterOwner = Cast<AAxeCharacterBase>(GetOwner());
	UAbilitySystemComponent* ASC = AxeCharacterOwner->GetAbilitySystemComponent();

	if (ASC)
	{
		ASC->RegisterGameplayTagEvent(EffectTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &UEffectNiagaraComponent::EffectTagChanged
		);
	}
	else
	{
		AxeCharacterOwner->OnAbilityInitOverDelegate.AddWeakLambda(this, [this, AxeCharacterOwner]()
		{
			UAbilitySystemComponent* InASC = AxeCharacterOwner->GetAbilitySystemComponent();
			InASC->RegisterGameplayTagEvent(EffectTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
				this, &UEffectNiagaraComponent::EffectTagChanged
			);
		});
	}
	AxeCharacterOwner->OnActorDeadDelegate.AddDynamic(this, &UEffectNiagaraComponent::OnAxeCharacterOwnerDeath);
}

void UEffectNiagaraComponent::EffectTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	const bool bAlive = Cast<IDeadInterface>(GetOwner())->IsAlive();

	if (NewCount > 0 && bAlive)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UEffectNiagaraComponent::OnAxeCharacterOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}
