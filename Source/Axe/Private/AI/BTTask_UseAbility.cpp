// wqz gas aura 


#include "AI/BTTask_UseAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"

EBTNodeResult::Type UBTTask_UseAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* Pawn = AIOwner->GetPawn();
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Pawn);
	if (!IsValid(ASC) || !AbilityTag.IsValid())
	{
		return EBTNodeResult::Failed;
	}
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);
	const FGameplayAbilitySpecHandle AbilitySpecHandle = AxeASC->GetAbilityHandleByAbilityTag(AbilityTag);
	const FGameplayAbilitySpec* AbilitySpec = AxeASC->FindAbilitySpecFromHandle(AbilitySpecHandle);
	const bool bIsActive = AbilitySpec->Ability->IsActive();
	if (bIsActive)
	{
		return EBTNodeResult::InProgress;
	}

	const bool bActivateAbility = AxeASC->TryActivateAbility(AbilitySpecHandle);
	if (!bActivateAbility)
	{
		return EBTNodeResult::Failed;
	}
	UGameplayAbility* PrimaryGameplayAbility = AbilitySpec->GetPrimaryInstance();
	PrimaryGameplayAbility->OnGameplayAbilityEnded.AddUObject(this, &UBTTask_UseAbility::OnAbilityEnded);

	return EBTNodeResult::InProgress;
}

void UBTTask_UseAbility::OnAbilityEnded(UGameplayAbility* Ability)
{
	FinishExecute(true);
}
