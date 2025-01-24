// wqz gas aura 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "EffectNiagaraComponent.generated.h"

class AAxeCharacterBase;
/**
 * 
 */
UCLASS()
class AXE_API UEffectNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UEffectNiagaraComponent();
	virtual void InitializeComponent() override;
	
	FGameplayTag GetEffectTag() const { return EffectTag; }
	void SetEffectTag(const FGameplayTag NewEffectTag) { EffectTag = NewEffectTag; }

protected:
	virtual void BeginPlay() override;

	void EffectTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UFUNCTION(blueprintcallable)
	void OnAxeCharacterOwnerDeath(AActor* DeadActor);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag EffectTag;
};
