#include "SingleGameMode.h"
#include "GameState/SingleGameState.h"
#include "LevelObjectManager.h"
#include "SpawnManager.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Controller/WitchController.h"
#include "OriginalSinPrj/GameInstance/UISubsystem.h"
#include "OriginalSinPrj/GameInstance/OriginalSinPrjGameInstance.h"
#include "OriginalSinPrj/Widget/LevelWidget/BattleWidget.h"

#include "AI/Player_AIController.h"

void ASingleGameMode::BeginPlay()
{
	LevelObjectManager = GetWorld()->SpawnActor<ALevelObjectManager>(LevelObjectManagerClass);
	SpawnManager = GetWorld()->SpawnActor<ASpawnManager>(SpawnManagerClass);

	PlayerControllerClass = AWitchController::StaticClass();
	GameStateClass = ASingleGameState::StaticClass();

	FTimerHandle DelayTimer;

	GetWorldTimerManager().SetTimer(
		DelayTimer,
		this,
		&ASingleGameMode::StartToSpawnActor,
		1.0f,
		false
	);
}

void ASingleGameMode::StartToSpawnActor()
{
	if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
		{
			Cast<UBattleWidget>(UISubsystem->CurrentActiveWidget)->ActivePlayerWidget();
			Cast<UBattleWidget>(UISubsystem->CurrentActiveWidget)->ActiveEnemyWidget();
		}
	}

	if (LevelObjectManager)
	{
		LevelObjectManager->InitializeTempObjects();
		LevelObjectManager->SpawnDeathZone();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelObjectManager NULL"));
	}

	SpawnPlayer();
	SpawnAI();
}

void ASingleGameMode::SpawnPlayer()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PC)
	{
		if (AWitchController* WitchController = Cast<AWitchController>(PC))
		{
			SpawnManager->SpawnPlayer(WitchController, FVector(0.0f, -250.0f, 1500.0f));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ASingleGameMode SpawnPlayer Fail"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ASingleGameMode PC None"));
	}
}

void ASingleGameMode::SpawnAI()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APlayer_AIController* CharacterAIController = GetWorld()->SpawnActor<APlayer_AIController>(
		APlayer_AIController::StaticClass(),
		FVector(0.0f, 0.0f, 0.0f),
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (CharacterAIController)
	{
		SpawnManager->SpawnMonster(CharacterAIController, FVector(0.0f, 250.0f, 1500.0f));
	}
}

void ASingleGameMode::ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation)
{
	if (ASingleGameState* SingleGameState = Cast<ASingleGameState>(UGameplayStatics::GetGameState(this)))
	{
		SingleGameState->ApplyDamage(Attacker, Damage, HitLocation);
	}
}

void ASingleGameMode::TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation)
{
	if (ASingleGameState* SingleGameState = Cast<ASingleGameState>(UGameplayStatics::GetGameState(this)))
	{
		SingleGameState->TakeDamage(Victim, Damage, HitLocation);
	}
}

void ASingleGameMode::OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation)
{
	if (ASingleGameState* SingleGameState = Cast<ASingleGameState>(UGameplayStatics::GetGameState(this)))
	{
		SingleGameState->OnDeathPlayer(Player, DeathLocation);
	}
}

void ASingleGameMode::OnDeathMonster(AActor* Monster, const FVector& DeathLocation)
{
	if (ASingleGameState* SingleGameState = Cast<ASingleGameState>(UGameplayStatics::GetGameState(this)))
	{
		SingleGameState->OnDeathMonster(Monster, DeathLocation);
	}
}
