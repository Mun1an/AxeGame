// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/DungeonGameMode.h"

#include "Actor/TargetPoint/EnemySpawnerTargetPoint.h"
#include "Character/AxeCharacterBase.h"
#include "Character/AxeCharacterEnemy.h"
#include "GameState/DungeonGameState.h"
#include "Kismet/GameplayStatics.h"

ADungeonGameMode::ADungeonGameMode()
{
	GameStateClass = ADungeonGameState::StaticClass();
}

void ADungeonGameMode::StartGame()
{
	bIsGameStarted = true;

	StartNewWave();
}

void ADungeonGameMode::StartNewWave()
{
	ADungeonGameState* DungeonGameState = GetGameState<ADungeonGameState>();
	if (DungeonGameState)
	{
		DungeonGameState->AddCurrentWaveIndex();
	}

	int32 CurrentWaveIndex = DungeonGameState->GetCurrentWaveIndex();

	// SpawnWaveEnemy
	SpawnWaveEnemy(CurrentWaveIndex);

	GEngine->AddOnScreenDebugMessage(
		-1, 5.f, FColor::Red,
		FString::Printf(TEXT("Wave %d"), CurrentWaveIndex)
	);
}

void ADungeonGameMode::OnWaveEnd()
{
	// 开始新的波次
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle, this, &ADungeonGameMode::StartNewWave, 5.f, false
	);
	// StartNewWave();

	GEngine->AddOnScreenDebugMessage(
		-1, 5.f, FColor::Green,
		FString::Printf(TEXT("Wave %d End"), GetGameState<ADungeonGameState>()->GetCurrentWaveIndex())
	);
}

void ADungeonGameMode::SpawnWaveEnemy(const int32 WaveIndex)
{
	SpawnedEnemyList.Empty();

	const FWaveInfo WaveInfo = GetWaveDataInfo(WaveIndex);
	if (WaveInfo.WaveEnemyInfo.Num() == 0)
	{
		return;
	}

	TArray<AActor*> EnemySpawnerList;
	GetAllEnemySpawner(EnemySpawnerList);

	for (const FWaveEnemyInfo& WaveEnemyInfo : WaveInfo.WaveEnemyInfo)
	{
		for (int i = 0; i < WaveEnemyInfo.EnemyCount; ++i)
		{
			if (WaveEnemyInfo.EnemyClass == nullptr)
			{
				continue;
			}
			const int32 RandomIndex = FMath::RandRange(0, EnemySpawnerList.Num() - 1);
			const AActor* EnemySpawner = EnemySpawnerList[RandomIndex];

			SpawnEnemy(WaveEnemyInfo.EnemyClass, EnemySpawner->GetTransform());
		}
	}
}

AActor* ADungeonGameMode::SpawnEnemy(TSubclassOf<AActor> EnemyClass, const FTransform& SpawnTransform)
{
	AAxeCharacterEnemy* SpawnEnemy = GetWorld()->SpawnActorDeferred<AAxeCharacterEnemy>(
		EnemyClass,
		SpawnTransform,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);
	SpawnEnemy->FinishSpawning(SpawnTransform);
	if (SpawnEnemy)
	{
		SpawnedEnemyList.Add(SpawnEnemy);
		AAxeCharacterBase* AxeCharacterBase = Cast<AAxeCharacterBase>(SpawnEnemy);
		if (AxeCharacterBase)
		{
			AxeCharacterBase->OnActorDeadDelegate.AddDynamic(this, &ADungeonGameMode::OnEnemyDead);
		}
	}
	return SpawnEnemy;
}

void ADungeonGameMode::OnEnemyDead(AActor* DeadActor)
{
	if (SpawnedEnemyList.Contains(DeadActor))
	{
		SpawnedEnemyList.Remove(DeadActor);
	}
	CheckCurrentWave();
}

FWaveInfo ADungeonGameMode::GetWaveDataInfo(const int32 WaveIndex) const
{
	if (WaveInfoDataTable)
	{
		const FName RowName = FName(*FString::FromInt(WaveIndex));
		FWaveInfo* WaveInfo = WaveInfoDataTable->FindRow<FWaveInfo>(RowName, TEXT(""), true);
		if (WaveInfo)
		{
			return *WaveInfo;	
		}
	}
	return FWaveInfo();
}

void ADungeonGameMode::CheckCurrentWave()
{
	if (SpawnedEnemyList.Num() <= 0)
	{
		OnWaveEnd();
	}
}

void ADungeonGameMode::GetAllEnemySpawner(TArray<AActor*>& OutEnemySpawner)
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawnerTargetPoint::StaticClass(), OutEnemySpawner);
}


void ADungeonGameMode::BeginPlay()
{
	Super::BeginPlay();

	// EnemySubsystem = GetGameInstance()->GetSubsystem<UEnemySubsystem>();
}
