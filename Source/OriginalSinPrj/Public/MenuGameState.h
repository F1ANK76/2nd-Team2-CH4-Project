#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "OriginalSinPrj/Interface/LobbyEvent.h"
#include "OriginalSinPrj/Interface/SessionManage.h"
#include "GameFramework/GameState.h"
#include "OriginalSinPrj/GameInstance/EnumSet.h"
#include "MenuGameState.generated.h"

class UAudioSubsystem;
struct FCharacterAudioDataStruct;
struct FBossAudioDataStruct;
struct FMonsterAudioDataStruct;

UCLASS()
class ORIGINALSINPRJ_API AMenuGameState : public AGameState, public ILobbyEvent, public ISessionManage
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

	UFUNCTION(BlueprintCallable)
	virtual void CreateGameSession(const FString& SessionName) override;

	//UFUNCTION(BlueprintCallable)
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


protected:
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable)
	void InitCharacterSounds();

	UFUNCTION(NetMulticast, Reliable)
	void InitBossSounds();

	UFUNCTION(NetMulticast, Reliable)
	void InitMonsterSounds();

	UFUNCTION(NetMulticast, Unreliable)
	void PlayCharacterSound(UAudioComponent* AudioComp, ECharacterSoundType SoundType);

	UFUNCTION(NetMulticast, Unreliable)
	void PlayBossSound(UAudioComponent* AudioComp, EBossSoundType SoundType);

	UFUNCTION(NetMulticast, Unreliable)
	void PlayMonsterSound(UAudioComponent* AudioComp, EMonsterSoundType SoundType);

	void PlaySound(UAudioComponent* AudioComp, USoundBase* SoundSource);

	bool LoadCharacterSoundSourceFromArray(ECharacterSoundType SoundType);
	bool LoadBossSoundSourceFromArray(EBossSoundType SoundType);
	bool LoadMonsterSoundSourceFromArray(EMonsterSoundType SoundType);
	bool CheckValidOfAudioHandle();

protected:
	UPROPERTY()
	TMap<ECharacterSoundType, USoundBase*> CharacterSoundMap;

	UPROPERTY()
	TMap<EBossSoundType, USoundBase*> BossSoundMap;

	UPROPERTY()
	TMap<EMonsterSoundType, USoundBase*> MonsterSoundMap;


	TArray<FCharacterAudioDataStruct*> CharacterSounds;
	TArray<FBossAudioDataStruct*> BossSounds;
	TArray<FMonsterAudioDataStruct*> MonsterSounds;

	UPROPERTY()
	TObjectPtr<UAudioSubsystem> AudioHandle = nullptr;

// ����
private: /*------------------------����------------------------*/

private: /*---------------------�����÷���---------------------*/


private: /*---------------------��Ƽ�÷���---------------------*/
	bool bIsSessionConnected;
	TMap<FString, bool> PlayerList;
};
