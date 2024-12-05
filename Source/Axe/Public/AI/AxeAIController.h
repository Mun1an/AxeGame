// wqz gas aura 

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AxeAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class AXE_API AAxeAIController : public AAIController
{
	GENERATED_BODY()

public:
	AAxeAIController();

protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
