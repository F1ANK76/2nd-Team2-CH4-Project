#include "GameInstance/MyOnlineSubsystem.h"
#include "MenuGameMode.h"

#include "OnlineSubsystem.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

void UMyOnlineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		OnlineSub->GetSessionInterface();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString::Printf(TEXT("Found Subsystem %s"), *OnlineSub->GetSubsystemName().ToString())
			);
		}

		Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->OnCreateSessionCompleteDelegates.AddUObject(this, &UMyOnlineSubsystem::OnCreateSessionComplete);
			Sessions->OnFindSessionsCompleteDelegates.AddUObject(this, &UMyOnlineSubsystem::OnFindGameSessionsComplete);
			Sessions->OnJoinSessionCompleteDelegates.AddUObject(this, &UMyOnlineSubsystem::OnJoinGameSessionComplete);
			Sessions->OnDestroySessionCompleteDelegates.AddUObject(this, &UMyOnlineSubsystem::OnDestroySessionComplete);
		}
	}
}

void UMyOnlineSubsystem::CreateGameSession(const FString& SessionName)
{
	if (OnlineSub)
	{
		if (Sessions.IsValid())
		{
			// 세션 설정 구성
			FOnlineSessionSettings SessionSettings;
			SessionSettings.bIsLANMatch = true;	// 데디케이트 서버 테스트 할 땐 false로 바꿔야됨
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
	if (OnlineSub)
	{
		if (Sessions.IsValid())
		{
			// 검색 설정 생성
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->bIsLanQuery = true;	// 데디케이트 서버 테스트 할 땐 false로 바꿔야됨
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->QuerySettings.Set(FName(TEXT("SEARCH_PRESENCE")), true, EOnlineComparisonOp::Equals);

			Sessions->FindSessions(0, SessionSearch.ToSharedRef());
		}
	}
}

void UMyOnlineSubsystem::JoinGameSession()
{
	if (OnlineSub)
	{
		if (Sessions.IsValid())
		{
			FString FoundSessionName;
			if (SessionSearch->SearchResults[0].Session.SessionSettings.Settings.Contains(FName("SESSION_NAME")))
			{
				FoundSessionName = SessionSearch->SearchResults[0].Session.SessionSettings.Settings[FName("SESSION_NAME")].Data.ToString();
			}


			Sessions->JoinSession(0, FName(FoundSessionName), SessionSearch->SearchResults[0]);
		}
	}
}

void UMyOnlineSubsystem::TravelMultiBattleMode()
{
	if (AMenuGameMode* MenuGameMode = Cast<AMenuGameMode>(GetWorld()->GetAuthGameMode()))
	{
		MenuGameMode->TravelLevel(FName(TEXT("/Game/Maps/TestLevel/TestMultiBattleLevel?listen")));
	}
}

void UMyOnlineSubsystem::JoinGameSession(const FOnlineSessionSearchResult& SearchResult)
{
}

void UMyOnlineSubsystem::DestroyGameSession()
{
	if (OnlineSub)
	{
		if (Sessions.IsValid())
		{
			Sessions->DestroySession(CurrentSessionName);
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
			CurrentSessionName = SessionName;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);
		}
		FString TravelURL;
		if (Sessions->GetResolvedConnectString(SessionName, TravelURL))
		{
			UE_LOG(LogTemp, Log, TEXT("세션 생성 성공! 접속 URL: %s"), *TravelURL);

			if (!TravelURL.Contains(TEXT("listen")))
			{
				// 이미 다른 쿼리 파라미터가 있다면 &를, 아니면 ?를 사용한다.
				if (TravelURL.Contains(TEXT("?")))
				{
					TravelURL.Append(TEXT("&listen"));
				}
				else
				{
					TravelURL.Append(TEXT("?listen"));
				}
			}

			if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
			{
				PC->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			}
		}
	}
	else
	{
		if (GEngine)
		{
			FString DebugMessage = FString::Printf(TEXT("Failed Create Session"));
			CurrentSessionName = SessionName;
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

		for (const FOnlineSessionSearchResult& Result : SessionSearch->SearchResults)
		{
			// "SESSION_NAME" 키가 존재하는지 확인
			if (Result.Session.SessionSettings.Settings.Contains(FName("SESSION_NAME")))
			{
				FString SessionName = Result.Session.SessionSettings.Settings[FName("SESSION_NAME")].Data.ToString();
				UE_LOG(LogTemp, Log, TEXT("Found Session: %s"), *SessionName);
			}
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

	if (OnlineSub)
	{
		if (Sessions.IsValid())
		{
			FString ConnectString;
			if (Sessions->GetResolvedConnectString(SessionName, ConnectString))
			{
				if (!ConnectString.Contains(TEXT("listen")))
				{
					// 이미 다른 쿼리 파라미터가 있다면 &를, 아니면 ?를 사용한다.
					if (ConnectString.Contains(TEXT("?")))
					{
						ConnectString.Append(TEXT("&listen"));
					}
					else
					{
						ConnectString.Append(TEXT("?listen"));
					}
				}

				APlayerController* PC = GetWorld()->GetFirstPlayerController();
				if (PC)
				{
					PC->ClientTravel(ConnectString, ETravelType::TRAVEL_Absolute);
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
