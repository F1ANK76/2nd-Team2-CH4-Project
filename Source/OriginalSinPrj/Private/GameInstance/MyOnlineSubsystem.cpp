#include "GameInstance/MyOnlineSubsystem.h"

#include "OnlineSubsystem.h"
#include "Engine/Engine.h"

void UMyOnlineSubsystem::CreateGameSession(const FString& SessionName)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->OnCreateSessionCompleteDelegates.AddUObject(this, &UMyOnlineSubsystem::OnCreateSessionComplete);

			// 세션 설정 구성
			FOnlineSessionSettings SessionSettings;
			SessionSettings.bIsLANMatch = false;
			SessionSettings.NumPublicConnections = 4;
			SessionSettings.bShouldAdvertise = true;
			SessionSettings.bUsesPresence = true;

			SessionSettings.Settings.Add(FName("SESSION_NAME"), FOnlineSessionSetting(SessionName, EOnlineDataAdvertisementType::ViaOnlineService));


			Sessions->CreateSession(0, FName(SessionName), SessionSettings);
		}
	}
}

void UMyOnlineSubsystem::FindGameSessions()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->OnFindSessionsCompleteDelegates.AddUObject(this, &UMyOnlineSubsystem::OnFindGameSessionsComplete);

			// 검색 설정 생성
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->bIsLanQuery = false;
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->QuerySettings.Set(FName(TEXT("SEARCH_PRESENCE")), true, EOnlineComparisonOp::Equals);

			Sessions->FindSessions(0, SessionSearch.ToSharedRef());
		}
	}
}

void UMyOnlineSubsystem::JoinGameSession(const FOnlineSessionSearchResult& SearchResult)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->OnJoinSessionCompleteDelegates.AddUObject(this, &UMyOnlineSubsystem::OnJoinGameSessionComplete);

			FString FoundSessionName;
			if (SearchResult.Session.SessionSettings.Settings.Contains(FName("SESSION_NAME")))
			{
				FoundSessionName = SearchResult.Session.SessionSettings.Settings[FName("SESSION_NAME")].Data.ToString();
			}


			Sessions->JoinSession(0, FName(FoundSessionName), SearchResult);
		}
	}
}

void UMyOnlineSubsystem::DestroyGameSession()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->OnDestroySessionCompleteDelegates.AddUObject(this, &UMyOnlineSubsystem::OnDestroySessionComplete);
		}
	}
}

void UMyOnlineSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (GEngine)
		{
			FString DebugMessage = FString::Printf(TEXT("Complete Create Session : %s"), *SessionName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);
		}
	}
}

void UMyOnlineSubsystem::OnFindGameSessionsComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (GEngine)
		{
			FString DebugMessage = FString::Printf(TEXT("Complete Find Sessions : %d"), SessionSearch->SearchResults.Num());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Failed Find Sessions"));
		}
	}
}

void UMyOnlineSubsystem::OnJoinGameSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (GEngine)
	{
		FString DebugMessage = FString::Printf(TEXT("Complete Join Session : %s"), *SessionName.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);
	}

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			FString ConnectString;
			if (Sessions->GetResolvedConnectString(SessionName, ConnectString))
			{
				APlayerController* PC = GetWorld()->GetFirstPlayerController();
				if (PC)
				{
					//PC->ClientTravel(ConnectString, ETravelType::TRAVEL_Absolute);
				}
			}
		}
	}
}

void UMyOnlineSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (GEngine)
		{
			FString DebugMessage = FString::Printf(TEXT("Complete Find Sessions : %s"), *SessionName.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);
		}
	}
}
