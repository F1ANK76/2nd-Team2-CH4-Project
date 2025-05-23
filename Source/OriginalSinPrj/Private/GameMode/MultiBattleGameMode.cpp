﻿#include "GameMode/MultiBattleGameMode.h"
#include "GameState/MultiBattleGameState.h"
#include "LevelObjectManager.h"
#include "SpawnManager.h"
#include "Player/Controller/WitchController.h"

#include "Kismet/GameplayStatics.h"

AMultiBattleGameMode::AMultiBattleGameMode()
	: CurrentActorArrayIndex(0)
	, LevelObjectManager(nullptr)
{
	GameStateClass = AMultiBattleGameState::StaticClass();
}

void AMultiBattleGameMode::BeginPlay()
{
	Super::BeginPlay();
	//CreateTestPlatform(FVector::ZeroVector, FRotator::ZeroRotator);

	LevelObjectManager = GetWorld()->SpawnActor<ALevelObjectManager>(LevelObjectManagerClass);
	SpawnManager = GetWorld()->SpawnActor<ASpawnManager>(SpawnManagerClass);
	StartDelay();
}

void AMultiBattleGameMode::StartDelay()
{
	FTimerHandle DelayTimer;

	GetWorldTimerManager().SetTimer(
		DelayTimer,
		this,
		&AMultiBattleGameMode::StartToSpawnActor,
		2.0f,
		false
	);
}

void AMultiBattleGameMode::StartToSpawnActor()
{
	InitializeTempObjects();
	LevelObjectManager->SpawnDeathZone();

	// 테스트 
	SpawnPlayer();
}

void AMultiBattleGameMode::SpawnPlayer()
{
	float DeltaY = 100.0f;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC)
		{
			if (AWitchController* WitchController = Cast<AWitchController>(PC))
			{
				UE_LOG(LogTemp, Warning, TEXT("컨트롤러 할당, 플레이어 스폰"));
				SpawnManager->SpawnPlayer(WitchController, FVector(0.0f, 0.0f + DeltaY, 100.0f));
				DeltaY += 100.0f;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("컨트롤러 캐스팅 실패"))
			}
		}
	}
}

void AMultiBattleGameMode::RespawnPlayer(APlayerController* PlayerController)
{
	/*
	플레이어 스테이트에서 플레이어의 정보 가져오기
	리스폰된 액터에 플레이어 정보 저장하기
	*/

	if (PlayerController)
	{
		FVector RespawnLocation;
		if (LevelObjectManager)
		{
			AActor* RespawnedPlatform = LevelObjectManager->GetRespawnPlatform();
			RespawnLocation = RespawnedPlatform->GetActorLocation();
		}
		if (AWitchController* WitchController = Cast<AWitchController>(PlayerController))
		{
			UE_LOG(LogTemp, Warning, TEXT("컨트롤러 할당, 플레이어 스폰"));
			SpawnManager->SpawnPlayer(WitchController, RespawnLocation);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("컨트롤러 캐스팅 실패"))
		}
	}
}

void AMultiBattleGameMode::ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation)
{
	/*
	범위 판정->TakeDamage함수로 HP를 감소시켜야하는 액터를 선정
	*/

	if (AMultiBattleGameState* MultiBattleGameState = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		//MultiBattleGameState->ApplyDamage(); -> 공격을 실행한 액터가 공격 애니메이션을 실행하도록 게임스테이트에서 명령
		//TakeDamage(); -> 공격 범위에 있던 액터의 HP를 감소시키고 피격 애니메이션을 실행하도록 명령
	}
}

void AMultiBattleGameMode::TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation)
{
	if (AMultiBattleGameState* MultiBattleGameState = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		MultiBattleGameState->TakeDamage(Victim, Damage, HitLocation);
	}
}

void AMultiBattleGameMode::OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation)
{
	if (AMultiBattleGameState* MultiBattleGameState = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		MultiBattleGameState->OnDeathPlayer(Player, DeathLocation);
	}
}

void AMultiBattleGameMode::OnDeathMonster(AActor* Monster, const FVector& DeathLocation)
{

	if (AMultiBattleGameState* MultiBattleGameState = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		MultiBattleGameState->OnDeathMonster(Monster, DeathLocation);
	}
}

void AMultiBattleGameMode::FinishMatch()
{
	if (AMultiBattleGameState* MultiBattleGameState = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		MultiBattleGameState->FinishMatch();
	}
}

void AMultiBattleGameMode::VictoryMatch()
{
	if (AMultiBattleGameState* MultiBattleGameState = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		MultiBattleGameState->VictoryMatch();
	}
}

void AMultiBattleGameMode::DefeatMatch()
{
	if (AMultiBattleGameState* MultiBattleGameState = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		MultiBattleGameState->DefeatMatch();
	}
}

void AMultiBattleGameMode::DrawMatch()
{
	if (AMultiBattleGameState* MultiBattleGameState = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		MultiBattleGameState->DrawMatch();
	}
}

void AMultiBattleGameMode::InitializeTempObjects()
{
	LevelObjectManager->InitializeTempObjects();
}

void AMultiBattleGameMode::SpawnAndDestroyObject()
{
	LevelObjectManager->SpawnAndDestroyObject();
}
