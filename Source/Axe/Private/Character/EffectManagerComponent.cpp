// wqz gas aura 


#include "Character/EffectManagerComponent.h"

#include "Character/EffectNiagaraComponent.h"
#include "GameplayTag/AxeGameplayTags.h"

UEffectManagerComponent::UEffectManagerComponent()
{
	bWantsInitializeComponent = true;

	FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();
	StunEffectComponent = CreateDefaultSubobject<UEffectNiagaraComponent>(TEXT("StunEffectComponent"));
	StunEffectComponent->SetEffectTag(AxeGameplayTags.Effect_Asset_Stun);
}

void UEffectManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UEffectManagerComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
	StunEffectComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
}
