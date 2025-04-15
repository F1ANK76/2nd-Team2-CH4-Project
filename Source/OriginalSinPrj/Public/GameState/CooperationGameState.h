// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "../Widget/LevelWidget/CooperationWidget.h"
#include "../Widget/AddedWidget/PlayerStateWidget.h"
#include "../Player/BaseWitch.h"
#include "OriginalSinPrj/Interface/CameraStateInterface.h"
#include "OriginalSinPrj/Interface/BattleEvent.h"
#include "OriginalSinPrj/Interface/MatchManage.h"
#include "OriginalSinPrj/GameInstance/OriginalSinPrjGameInstance.h"
#include "CooperationGameState.generated.h"

struct FBuffType;
class ACooperationGameMode;


UCLASS()
class ORIGINALSINPRJ_API ACooperationGameState : public AGameState, public ICameraStateInterface, public IBattleEvent
{
	GENERATED_BODY()

public:
    UOriginalSinPrjGameInstance* GameInstance = nullptr;
    
protected:
    ACooperationGameState();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    
    //리플리케이트 함수
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
    //카메라 처리용 함수.
    virtual FVector GetCameraLocation() const override { return CameraLocation; }
    virtual FRotator GetCameraRotation() const override { return CameraRotation; }
    virtual float GetCameraDistance() const override { return CameraDistance; }
    
    //카메라 위치 설정하기... //
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
    //보스전 타이머 켜기
    void TurnOnTimer(); 

    void RegisterInitialController(APlayerController* PC);

    void InitPlayerInfo();
    void UpdatePlayerInfo();
    void InitPlayerUIInfo();
    void UpdatePlayerUIInfo();


    UPROPERTY(ReplicatedUsing = OnRep_TurnOnStageUI)
    int CurrentStageIndex = 0;

    void TurnOnStage1Widget();
    void TurnOnStage2Widget();
    void TurnOnStage3Widget();

    UFUNCTION()
    void OnRep_TurnOnStageUI();

    void RequestPlayerToOpenBuffUI();//플레이어에게 버프 선택 UI 열도록 시키기

    void RequestPlayerToOpenResultUI(); //플레이어에게 결과 UI 열도록 시키기

    void ReceiveSelectedBuff(APlayerController* player, FBuffType* Bufftype); // 플레이어 UI에서 선택된 버프 내용 받고 어디다가 저장시켜놓자.



private:

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UCooperationWidget> CooperationWidgetClass; // UI 클래스


    FTimerHandle TimerHandle;
    void CheckLevelUp(AActor* Player);

public:
    void SetPlayerPawn(ABaseWitch* InPawn);

    ACooperationGameMode* CooperationGameGameMode;

    UPROPERTY(Replicated)
    bool bPlayer1SelectedBuff = false;

    UPROPERTY(Replicated)
    bool bPlayer2SelectedBuff = false;


    void SetPlayerMove(bool bCanMove);


    UPROPERTY(ReplicatedUsing = OnRep_SetPlayerMove)
    bool bIsPlayerCanMove = true;

    UFUNCTION()
    void OnRep_SetPlayerMove();

    UPROPERTY(Replicated)
    bool bIsStage3Started;

    UPROPERTY(ReplicatedUsing = OnRep_UpdateTimer)
    float Timer;

    UFUNCTION()
    void OnRep_UpdateTimer();

    void UpdateTimer();

    //////////////////////////////////////////////////////////// UI와 연동하는 함수////////////////////////////////////////////////////
    // Player 정보 관리
    UPROPERTY(BlueprintReadOnly)
    TMap<AActor*, FPlayerData> PlayerInfos;

    UPROPERTY(ReplicatedUsing = OnRep_UpdatePlayerDataUI)
    TArray<FPlayerData> PlayerDatas;

    bool bIsPlayerDataUpdated = false;

    UFUNCTION()
    void OnRep_UpdatePlayerDataUI();

    ///////////////////


    //플레이어 컨트롤러 저장해놓기
    UPROPERTY()
    TArray<TWeakObjectPtr<APlayerController>> PlayerControllerSet;
    

    FBuffType* Player1Stage1SelectedBuff;
    FBuffType* Player2Stage1SelectedBuff;

    FBuffType* Player1Stage2SelectedBuff;
    FBuffType* Player2Stage2SelectedBuff;
    

    void ApplyBuffStat(); // 게임모드가 허락해준 버프 적용시키기

    void AddExperienceToPlayer(AActor* Player, int32 Amount);

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
    void Multicast_OnDeathPlayer(ACharacter* Player, const FVector& DeathLocatio);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_OnDeathMonster(AActor* Monster, const FVector& DeathLocation);
};
