// wqz gas aura 


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Character/AxeCharacterPlayer.h"
#include "Interface/EnemyInterface.h"
#include "Kismet/GameplayStatics.h"

UBTService_FindNearestPlayer::UBTService_FindNearestPlayer()
{
	SearchObjectTypes.AddUnique(UEngineTypes::ConvertToObjectType(ECC_Pawn));
}

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwningPawn = AIOwner->GetPawn();

	TArray<AActor*> TargetActors;

	UKismetSystemLibrary::SphereOverlapActors(
		OwningPawn,
		OwningPawn->GetActorLocation(),
		SearchRadius,
		SearchObjectTypes,
		AAxeCharacterPlayer::StaticClass(),
		{},
		TargetActors
	);

	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;
	for (AActor* TagActor : TargetActors)
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
