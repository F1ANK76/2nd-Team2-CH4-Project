#include "MenuGameMode.h"
#include "MenuGameState.h"
#include "OnlineSessionSettings.h"

#include "Kismet/GameplayStatics.h"
#include "Player/Controller/WitchController.h"
#include "OriginalSinPrj/GameInstance/OriginalSinPrjGameInstance.h"

void AMenuGameMode::ReadyPlayer(APlayerController* Player)
{
	if (AMenuGameState* MenuGameState = Cast<AMenuGameState>(UGameplayStatics::GetGameState(this)))
	{
		MenuGameState->ReadyPlayer(Player);
	}
}

void AMenuGameMode::UnreadyPlayer(APlayerController* Player)
{
	if (AMenuGameState* MenuGameState = Cast<AMenuGameState>(UGameplayStatics::GetGameState(this)))
	{
		MenuGameState->UnreadyPlayer(Player);
	}
}

void AMenuGameMode::SelectCharacter(const FName& CharacterName, APlayerController* Player)
{
	if (AMenuGameState* MenuGameState = Cast<AMenuGameState>(UGameplayStatics::GetGameState(this)))
	{
		MenuGameState->SelectCharacter(CharacterName, Player);
	}
}

void AMenuGameMode::SelectMap(const FName& MapName)
{
	if (AMenuGameState* MenuGameState = Cast<AMenuGameState>(UGameplayStatics::GetGameState(this)))
	{
		MenuGameState->SelectMap(MapName);
	}
}

void AMenuGameMode::TravelLevel(const FName& LevelName)
{
	if (HasAuthority())
	{
		GetWorld()->SeamlessTravel(LevelName.ToString());
	}
}

void AMenuGameMode::JoinSessionPlayer(const FString& PlayerName)
{
	if (AMenuGameState* MenuGameState = Cast<AMenuGameState>(UGameplayStatics::GetGameState(this)))
	{
		MenuGameState->JoinSessionPlayer(PlayerName);
	}
}

void AMenuGameMode::ExitSessionPlayer(const FString& PlayerName)
{
	if (AMenuGameState* MenuGameState = Cast<AMenuGameState>(UGameplayStatics::GetGameState(this)))
	{
		MenuGameState->ExitSessionPlayer(PlayerName);
	}
}

void AMenuGameMode::CreateGameSession(const FString& SessionName)
{
	if (AMenuGameState* MenuGameState = Cast<AMenuGameState>(UGameplayStatics::GetGameState(this)))
	{
		MenuGameState->CreateGameSession(SessionName);
	}
}

void AMenuGameMode::JoinGameSession(const FOnlineSessionSearchResult& SearchResult)
{
	if (AMenuGameState* MenuGameState = Cast<AMenuGameState>(UGameplayStatics::GetGameState(this)))
	{
		MenuGameState->JoinGameSession(SearchResult);
	}
}

void AMenuGameMode::FindGameSessions()
{
	if (AMenuGameState* MenuGameState = Cast<AMenuGameState>(UGameplayStatics::GetGameState(this)))
	{
		MenuGameState->FindGameSessions();
	}
}

void AMenuGameMode::DestroyGameSession()
{
	if (AMenuGameState* MenuGameState = Cast<AMenuGameState>(UGameplayStatics::GetGameState(this)))
	{
		MenuGameState->DestroyGameSession();
	}
}

void AMenuGameMode::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AWitchController* PC = Cast<AWitchController>(*It);

		if (IsValid(PC))
		{
			PC->ResponseShowLevelWidget();
		}
	}
}

void AMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	bUseSeamlessTravel = true;
}

void AMenuGameMode::StartMultiGame()
{
	// TODO : Check All Client Readied
	// TODO : Check Cooper or Multi
	UE_LOG(LogTemp, Warning, TEXT("Response Start Multi Game"));
	if (!IsValid(GetGameInstance()))
	{
		return;
	}

	UOriginalSinPrjGameInstance* GameInstance = GetGameInstance<UOriginalSinPrjGameInstance>();

	if (!IsValid(GameInstance))
	{
		return;
	}

	// if Selected Mode is Multi
	GameInstance->RequestOpenLevelByType(ELevelType::FarmingLevel, false);

	// if Selected Mode is Cooper
}
