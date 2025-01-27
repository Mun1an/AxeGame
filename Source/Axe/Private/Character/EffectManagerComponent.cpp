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

	FireBurnEffectComponent = CreateDefaultSubobject<UEffectNiagaraComponent>(TEXT("FireBurnEffectComponent"));
	FireBurnEffectComponent->SetEffectTag(AxeGameplayTags.Effect_Asset_FireBurn);
}

void UEffectManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UEffectManagerComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
	StunEffectComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	FireBurnEffectComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
}
