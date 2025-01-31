// wqz gas aura 

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_UseAbility.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS()
class AXE_API UBTTask_UseAbility : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AbilityTag;

protected:
	void OnAbilityEnded(UGameplayAbility* Ability);

private:
	FGameplayAbilitySpecHandle ActiveAbilitySpecHandle;
};
