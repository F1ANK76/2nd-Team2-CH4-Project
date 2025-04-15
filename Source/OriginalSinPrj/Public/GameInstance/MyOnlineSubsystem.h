#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OriginalSinPrj/Interface/SessionManage.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MyOnlineSubsystem.generated.h"

UCLASS()
class ORIGINALSINPRJ_API UMyOnlineSubsystem : public UGameInstanceSubsystem, public ISessionManage
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable)
	virtual void CreateGameSession(const FString& SessionName) override;

	UFUNCTION(BlueprintCallable)
	virtual void FindGameSessions() override;

	UFUNCTION(BlueprintCallable)
	void JoinGameSession();

	UFUNCTION(BlueprintCallable)
	void TravelMultiBattleMode();

	virtual void JoinGameSession(const FOnlineSessionSearchResult& SearchResult) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void DestroyGameSession() override;

private:

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindGameSessionsComplete(bool bWasSuccessful);
	void OnJoinGameSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	FName CurrentSessionName;
	IOnlineSubsystem* OnlineSub;
	IOnlineSessionPtr Sessions;
};
