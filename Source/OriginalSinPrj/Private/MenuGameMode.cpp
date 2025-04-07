#include "MenuGameMode.h"
#include "MenuGameState.h"

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
	if (AMenuGameState* MenuGameState = Cast<AMenuGameState>(UGameplayStatics::GetGameState(this)))
	{
		MenuGameState->TravelLevel(LevelName);
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
