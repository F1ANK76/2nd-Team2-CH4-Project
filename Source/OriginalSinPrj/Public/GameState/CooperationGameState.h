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

    void TurnOnTimer(); //������ Ÿ�̸� �ѱ�

    // Player ���� ����
    UPROPERTY(BlueprintReadOnly)
    TMap<APlayerController*, FPlayerData> PlayerInfos;


    void InitPlayerInfo();
    void UpdatePlayerInfo();
    void InitPlayerUIInfo();
    void UpdatePlayerUIInfo();

    void RequestPlayerToOpenBuffUI();//�÷��̾�� ���� ���� UI ������ ��Ű��

    void ReceiveSelectedBuff(EBuffType Bufftype); // �÷��̾� UI���� ���õ� ���� ���� �ޱ�
    
    void ApplyBuffStat(EBuffType Bufftype); // ���Ӹ�尡 ������� ���� �����Ű��

    void AddExperienceToPlayer(APlayerController* Player, int32 Amount);




private:

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UBattleWidget> BattleWidgetClass; // UI Ŭ����


    FTimerHandle TimerHandle;
    void CheckLevelUp(APlayerController* Player);

public:
    void SetPlayerPawn(ABaseWitch* InPawn);
private:
    UPROPERTY()
    ABaseWitch* PlayerPawnRef;
	*/
};
