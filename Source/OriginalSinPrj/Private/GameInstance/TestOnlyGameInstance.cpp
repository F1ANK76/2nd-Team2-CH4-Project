#include "GameInstance/TestOnlyGameInstance.h"
#include "GameInstance/MyOnlineSubsystem.h"

void UTestOnlyGameInstance::Init()
{
	Super::PostInitProperties();

	OnlineSubsystem = GetSubsystem<UMyOnlineSubsystem>();
}

void UTestOnlyGameInstance::CreateGameSession(const FString& SessionName)
{
	if (OnlineSubsystem)
	{
		OnlineSubsystem->CreateGameSession(SessionName);
	}
}

void UTestOnlyGameInstance::JoinGameSession(const FOnlineSessionSearchResult& SearchResult)
{
	if (OnlineSubsystem)
	{
		OnlineSubsystem->JoinGameSession(SearchResult);
	}
}

void UTestOnlyGameInstance::FindGameSessions()
{
	if (OnlineSubsystem)
	{
		OnlineSubsystem->FindGameSessions();
	}
}

void UTestOnlyGameInstance::DestroyGameSession()
{
	if (OnlineSubsystem)
	{
		OnlineSubsystem->DestroyGameSession();
	}
}
