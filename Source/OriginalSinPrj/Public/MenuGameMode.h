#pragma once

#include "CoreMinimal.h"
#include "OriginalSinPrj/Interface/LobbyEvent.h"
#include "OriginalSinPrj/Interface/SessionManage.h"
#include "GameFramework/GameMode.h"
#include "MenuGameMode.generated.h"

UCLASS()
class ORIGINALSINPRJ_API AMenuGameMode : public AGameMode, public ILobbyEvent, public ISessionManage
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void ReadyPlayer(APlayerController* Player) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void UnreadyPlayer(APlayerController* Player) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void SelectCharacter(const FName& CharacterName, APlayerController* Player) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void SelectMap(const FName& MapName) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void TravelLevel(const FName& LevelName) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void JoinSessionPlayer(const FString& PlayerName) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void ExitSessionPlayer(const FString& PlayerName) override;

	UFUNCTION(BlueprintCallable)
	virtual void CreateGameSession(const FString& SessionName) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void JoinGameSession(const FOnlineSessionSearchResult& SearchResult) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void FindGameSessions() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void DestroyGameSession() override;
};