#include "MenuGameMode.h"
#include "MenuGameState.h"
#include "OnlineSessionSettings.h"

#include "Kismet/GameplayStatics.h"

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
