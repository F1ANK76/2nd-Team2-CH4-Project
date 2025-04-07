#pragma once

#include "CoreMinimal.h"
#include "OriginalSinPrj/Interface/LobbyEvent.h"
#include "GameFramework/GameState.h"
#include "MenuGameState.generated.h"

UCLASS()
class ORIGINALSINPRJ_API AMenuGameState : public AGameState, public ILobbyEvent
{
	GENERATED_BODY()
	
// �Լ�
public: /*------------------------����------------------------*/
	virtual void TravelLevel(const FName& LevelName) override;
	virtual void SelectCharacter(const FName& CharacterName, APlayerController* Player) override;
	virtual void SelectMap(const FName& MapName) override;

public: /*---------------------�����÷���---------------------*/
	
	UFUNCTION(BlueprintCallable)
	void Local_TravelLevel(const FName& LevelName);

	UFUNCTION(BlueprintCallable)
	void Local_SelectCharacter(const FName& CharacterName, APlayerController* Player);

public: /*---------------------��Ƽ�÷���---------------------*/

	virtual void ReadyPlayer(APlayerController* Player) override;
	virtual void UnreadyPlayer(APlayerController* Player) override;
	virtual void JoinSessionPlayer(const FString& PlayerName) override;
	virtual void ExitSessionPlayer(const FString& PlayerName) override;

	UFUNCTION(BlueprintPure, Category = "Session")
	bool GetIsSessionConnected() const;

	UFUNCTION(BlueprintCallable, Category = "Session")
	void SetIsSessionConnected(bool IsSessionConnected);

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

// ����
private: /*------------------------����------------------------*/

private: /*---------------------�����÷���---------------------*/


private: /*---------------------��Ƽ�÷���---------------------*/
	bool bIsSessionConnected;
	TMap<FString, bool> PlayerList;
};
