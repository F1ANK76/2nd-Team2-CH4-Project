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
    AFarmingGameState();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;


    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(Replicated)
    bool bIsFarmingStarted;

    UPROPERTY(Replicated, BlueprintReadOnly)
    float TimeRemaining;

    // Player ���� ����
    UPROPERTY(BlueprintReadOnly)
    TMap<APlayerController*, FPlayerData> PlayerInfos;


    void InitPlayerInfo();
    void UpdatePlayerInfo();
    void InitPlayerUIInfo();
    void UpdatePlayerUIInfo();

    void AddExperienceToPlayer(APlayerController* Player, int32 Amount);

    void StartFarmingMode();

    void EndFarmingMode(); // 0�� ���� �� ó���� �Լ�
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
