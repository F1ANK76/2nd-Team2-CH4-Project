// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "../Widget/LevelWidget/CooperationWidget.h"
#include "../Widget/AddedWidget/BuffSelectWidget.h"
#include "../Widget/AddedWidget/ResultWidget.h"
#include "../Widget/AddedWidget/PlayerStateWidget.h"
#include "../Player/BaseWitch.h"
#include "OriginalSinPrj/Interface/CameraStateInterface.h"
#include "OriginalSinPrj/Interface/BattleEvent.h"
#include "OriginalSinPrj/Interface/MatchManage.h"
#include "OriginalSinPrj/GameInstance/OriginalSinPrjGameInstance.h"
#include "OriginalSinPrj/GameInstance/EnumSet.h"
#include "OriginalSinPrj/Public/Player/Struct/CharacterStateBuffer.h"
#include "CooperationGameState.generated.h"


class ACooperationGameMode;
class UAudioSubsystem;
struct FBuffType;
struct FCharacterAudioDataStruct;
struct FBossAudioDataStruct;
struct FMonsterAudioDataStruct;

UCLASS()
class ORIGINALSINPRJ_API ACooperationGameState : public AGameState, public ICameraStateInterface, public IBattleEvent
{
	GENERATED_BODY()

public:
    UOriginalSinPrjGameInstance* GameInstance = nullptr;
    ACooperationGameMode* CooperationGameGameMode;
    

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TObjectPtr<UBaseWidget> BuffSelectWidget;

    TArray<EBuffType> BuffUIInit();

public:
    UFUNCTION(NetMulticast, Unreliable)
    void PlayCharacterSound(UAudioComponent* AudioComp, ECharacterSoundType SoundType);

    UFUNCTION(NetMulticast, Unreliable)
    void PlayBossSound(UAudioComponent* AudioComp, EBossSoundType SoundType);

    UFUNCTION(NetMulticast, Unreliable)
    void PlayMonsterSound(UAudioComponent* AudioComp, EMonsterSoundType SoundType);

protected:
    UFUNCTION(NetMulticast, Reliable)
    void InitCharacterSounds();

    UFUNCTION(NetMulticast, Reliable)
    void InitBossSounds();

    UFUNCTION(NetMulticast, Reliable)
    void InitMonsterSounds();

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

protected:
    ACooperationGameState();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    
    //���ø�����Ʈ �Լ�
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:

    void SetPlayerPawn(ABaseWitch* InPawn);



    //ī�޶� ó���� �Լ�.
    virtual FVector GetCameraLocation() const override { return CameraLocation; }
    virtual FRotator GetCameraRotation() const override { return CameraRotation; }
    virtual float GetCameraDistance() const override { return CameraDistance; }
    
    //ī�޶� ��ġ �����ϱ�... //
    void SetStage1CameraTransform();
    void SetStage2CameraTransform();
    void SetStage3CameraTransform();

protected:
    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Camera")
    FVector CameraLocation;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Camera")
    FRotator CameraRotation;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Camera")
    float CameraDistance;

public:
    //������ Ÿ�̸� �ѱ�
    void TurnOnTimer(); 
    void TurnOffTimer();
    void RegisterInitialController(APlayerController* PC);

    void InitPlayerInfo();
    void UpdatePlayerInfo(const FCharacterStateBuffer& State);
    void InitPlayerUIInfo();
    void UpdatePlayerUIInfo();


    UPROPERTY(ReplicatedUsing = OnRep_TurnOnStageUI)
    int CurrentStageIndex = 0;

    UFUNCTION()
    void OnRep_TurnOnStageUI();
    
    void TurnOnStage1Widget();
    void TurnOnStage2Widget();
    void TurnOnStage3Widget();
    void TurnOffStage3Widget();


    UPROPERTY(ReplicatedUsing = OnRep_UpdatePlayerInitData)
    int PlayerDataChanged = 0;

    UFUNCTION()
    void OnRep_UpdatePlayerInitData();



    UPROPERTY(Replicated)
    FPlayerData Player1StateData;

    UPROPERTY(Replicated)
    FPlayerData Player2StateData;




    void TurnOnResultWidget();

    void CreateBuffSelectUI();
    void ReceiveSelectedBuff(APlayerController* player, FBuffType* Bufftype); // �÷��̾� UI���� ���õ� ���� ���� �ް� ���ٰ� ������ѳ���.
    void CloseBuffSelectUI();

    UPROPERTY(ReplicatedUsing = OnRep_TurnOffBuffUI)
    int SelectBuffPlayer = 0;

    UFUNCTION()
    void OnRep_TurnOffBuffUI();

    UPROPERTY(Replicated)
    int bIsPlayerBuffSelect = 0;


    void CheckLevelUp(AActor* Player);


    void SetPlayerMove(bool bCanMove);

    UPROPERTY(ReplicatedUsing = OnRep_SetPlayerMove)
    bool bIsPlayerCanMove = true;

    UFUNCTION()
    void OnRep_SetPlayerMove();

    //////////////////////////////////////////////////////////// UI�� �����ϴ� �Լ�////////////////////////////////////////////////////
    // Player ���� ����
    UPROPERTY(BlueprintReadOnly)
    TMap<AActor*, FPlayerData> PlayerInfos;

    UPROPERTY(ReplicatedUsing = OnRep_UpdatePlayer1DataUI)
    int Player1DataChanged = 0;

    UFUNCTION()
    void OnRep_UpdatePlayer1DataUI();

    UPROPERTY(ReplicatedUsing = OnRep_UpdatePlayer2DataUI)
    int Player2DataChanged = 0;

    UFUNCTION()
    void OnRep_UpdatePlayer2DataUI();


    ///////////////////
    //�÷��̾� ��Ʈ�ѷ� �����س���
    UPROPERTY()
    TArray<TWeakObjectPtr<APlayerController>> PlayerControllerSet;
    
    UPROPERTY()
    TArray<EBuffType> SelectedBuff;

    FBuffType* Player1Stage1SelectedBuff;
    FBuffType* Player2Stage1SelectedBuff;

    FBuffType* Player1Stage2SelectedBuff;
    FBuffType* Player2Stage2SelectedBuff;
    
    void ApplyBuffStat(); // ���Ӹ�尡 ������� ���� �����Ű��

    void AddExperienceToPlayer(AActor* Player, int32 Amount);



    //////�������//////
    UPROPERTY(ReplicatedUsing = OnRep_UpdateBossDataUI)
    TArray<FBossUIData> BossData; //�ʱ�ȭ �ʿ�

    UFUNCTION()
    void OnRep_UpdateBossDataUI();

    void UpdateBossDataUI();


   

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UPROPERTY(Replicated)
    float Player1ReceivedDamage = 0;

    UPROPERTY(Replicated)
    float Player2ReceivedDamage = 0;

    UPROPERTY(Replicated)
    int32 Player1DeathCount = 0;

    UPROPERTY(Replicated)
    int32 Player2DeathCount = 0;


    UPROPERTY(Replicated)
    int32 Player1ApplyAttackCount = 0;

    UPROPERTY(Replicated)
    int32 Player2ApplyAttackCount = 0;

    UPROPERTY(Replicated)
    float SpendedStage1Timer = 0;

    UPROPERTY(Replicated)
    float SpendedStage2Timer = 0;
    //Timer

    UPROPERTY(Replicated)
    bool bIsStage1Reached = false;

    UPROPERTY(Replicated)
    bool bIsStage2Reached = false;

    UPROPERTY(Replicated)
    bool bIsStage3Reached = false;

    UPROPERTY(Replicated)
    bool bIsStage1Started = false;

    UPROPERTY(Replicated)
    bool bIsStage2Started = false;

    UPROPERTY(Replicated)
    bool bIsStage3Started = false;

 


    UPROPERTY(ReplicatedUsing = OnRep_UpdateTimer)
    float SpendedStage3Timer;

    UFUNCTION()
    void OnRep_UpdateTimer();

    void UpdateTimer();
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:
    UPROPERTY()
    ABaseWitch* PlayerPawnRef;
	
public:
    virtual void ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation) override;
    virtual void TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation) override;
    virtual void OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation) override;
    virtual void OnDeathMonster(AActor* Monster, const FVector& DeathLocation) override;

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_OnDeathMonster(AActor* Monster, const FVector& DeathLocation);
};
