// wqz gas aura 


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Interface/EnemyInterface.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwningPawn = AIOwner->GetPawn();

	TArray<AActor*> TagActors;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, FName("Player"), TagActors);

	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;
	for (AActor* TagActor : TagActors)
	{
		if (IsValid(TagActor) && IsValid(OwningPawn))
		{
			const float Distance = OwningPawn->GetDistanceTo(TagActor);
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = TagActor;
			}
		}
	}
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);

	// SetCombatTarget
	if (IEnemyInterface* EnemyInterface = Cast<IEnemyInterface>(OwningPawn))
	{
		EnemyInterface->SetCombatTarget(ClosestActor);
	}
}
