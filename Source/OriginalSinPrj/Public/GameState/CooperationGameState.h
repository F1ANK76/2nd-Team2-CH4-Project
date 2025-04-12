// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "../Widget/LevelWidget/CooperationWidget.h"
#include "../Widget/AddedWidget/PlayerStateWidget.h"
#include "../Player/BaseWitch.h"
#include "CooperationGameState.generated.h"


UCLASS()
class ORIGINALSINPRJ_API ACooperationGameState : public AGameState
{
	GENERATED_BODY()

public:
    /*
    UPROPERTY(Replicated)
    bool bPlayer1SelectedBuff = false;

    UPROPERTY(Replicated)
    bool bPlayer2SelectedBuff = false;
    */
    /*
    ACooperationGameState();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;


    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(Replicated)
    bool bIsStage3Started;

    UPROPERTY(Replicated, BlueprintReadOnly)
    float Timer;

    void TurnOnTimer(); //보스전 타이머 켜기

    // Player 정보 관리
    UPROPERTY(BlueprintReadOnly)
    TMap<APlayerController*, FPlayerData> PlayerInfos;


    void InitPlayerInfo();
    void UpdatePlayerInfo();
    void InitPlayerUIInfo();
    void UpdatePlayerUIInfo();

    void RequestPlayerToOpenBuffUI();//플레이어에게 버프 선택 UI 열도록 시키기

    void ReceiveSelectedBuff(EBuffType Bufftype); // 플레이어 UI에서 선택된 버프 내용 받기
    
    void ApplyBuffStat(EBuffType Bufftype); // 게임모드가 허락해준 버프 적용시키기

    void AddExperienceToPlayer(APlayerController* Player, int32 Amount);




private:

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UBattleWidget> BattleWidgetClass; // UI 클래스


    FTimerHandle TimerHandle;
    void CheckLevelUp(APlayerController* Player);

public:
    void SetPlayerPawn(ABaseWitch* InPawn);
private:
    UPROPERTY()
    ABaseWitch* PlayerPawnRef;
	*/
};
