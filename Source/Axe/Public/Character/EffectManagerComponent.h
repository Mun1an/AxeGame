// wqz gas aura 

#pragma once

#include "CoreMinimal.h"
#include "EffectManagerComponent.generated.h"

class UEffectNiagaraComponent;
/**
 * 
 */
UCLASS()
class AXE_API UEffectManagerComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UEffectManagerComponent();
	virtual void InitializeComponent() override;
	virtual void OnComponentCreated() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	UEffectNiagaraComponent* StunEffectComponent;
};
