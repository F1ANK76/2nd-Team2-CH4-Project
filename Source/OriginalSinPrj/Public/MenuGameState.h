#pragma once

#include "CoreMinimal.h"
#include "OriginalSinPrj/Interface/LobbyEvent.h"
#include "OriginalSinPrj/Interface/SessionManage.h"
#include "GameFramework/GameState.h"
#include "MenuGameState.generated.h"

UCLASS()
class ORIGINALSINPRJ_API AMenuGameState : public AGameState, public ILobbyEvent, public ISessionManage
{
	GENERATED_BODY()
	
// 함수
public: /*------------------------공통------------------------*/
	virtual void TravelLevel(const FName& LevelName) override;
	virtual void SelectCharacter(const FName& CharacterName, APlayerController* Player) override;
	virtual void SelectMap(const FName& MapName) override;

public: /*---------------------로컬플레이---------------------*/
	
	UFUNCTION(BlueprintCallable)
	void Local_TravelLevel(const FName& LevelName);

	UFUNCTION(BlueprintCallable)
	void Local_SelectCharacter(const FName& CharacterName, APlayerController* Player);

public: /*---------------------멀티플레이---------------------*/

	virtual void ReadyPlayer(APlayerController* Player) override;
	virtual void UnreadyPlayer(APlayerController* Player) override;
	virtual void JoinSessionPlayer(const FString& PlayerName) override;
	virtual void ExitSessionPlayer(const FString& PlayerName) override;

	UFUNCTION(BlueprintPure, Category = "Session")
	bool GetIsSessionConnected() const;

	UFUNCTION(BlueprintCallable, Category = "Session")
	void SetIsSessionConnected(bool IsSessionConnected);

	UFUNCTION(BlueprintCallable)
	virtual void CreateGameSession(const FString& SessionName) override;

	UFUNCTION(BlueprintCallable)
	virtual void JoinGameSession(const FOnlineSessionSearchResult& SearchResult) override;

	UFUNCTION(BlueprintCallable)
	virtual void FindGameSessions() override;

	UFUNCTION(BlueprintCallable)
	virtual void DestroyGameSession() override;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_TravelLevel(const FName& LevelName);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SelectCharacter(const FName& CharacterName, APlayerController* Player);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReadyPlayer(APlayerController* Player);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UnreadyPlayer(APlayerController* Player);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_JoinSessionPlayer(const FString& PlayerName);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ExitSessionPlayer(const FString& PlayerName);

// 변수
private: /*------------------------공통------------------------*/

private: /*---------------------로컬플레이---------------------*/


private: /*---------------------멀티플레이---------------------*/
	bool bIsSessionConnected;
	TMap<FString, bool> PlayerList;
};
