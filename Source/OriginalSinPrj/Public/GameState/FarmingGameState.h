// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OriginalSinPrj/Interface/BattleEvent.h"
#include "../Widget/LevelWidget/BattleWidget.h"
#include "../Widget/AddedWidget/PlayerStateWidget.h"
#include "../Player/BaseWitch.h"
#include "OriginalSinPrj/GameInstance/OriginalSinPrjGameInstance.h"
#include "GameFramework/GameState.h"
#include "FarmingGameState.generated.h"

class AFarmingGameMode;

UCLASS()
class ORIGINALSINPRJ_API AFarmingGameState : public AGameState, public IBattleEvent
{
	GENERATED_BODY()
	
    
public:
    AFarmingGameState();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    AFarmingGameMode* FarmingGameMode;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(Replicated)
    bool bIsFarmingStarted;


    UPROPERTY(ReplicatedUsing = OnRep_UpdateTimer)
    float TimeRemaining;

    UFUNCTION()
    void OnRep_UpdateTimer();

    void UpdateTimer();


    // Player 정보 관리
    UPROPERTY(BlueprintReadOnly)
    TMap<APlayerController*, FPlayerData> PlayerInfos;


    void TurnOnPlayerUI();
    void TurnOnAllUI();

    UPROPERTY(ReplicatedUsing = OnRep_TurnOnAllUI)
    int MultiPlayer = 0;
    
    UFUNCTION()
    void OnRep_TurnOnAllUI();

    void InitPlayerInfo();
    void UpdatePlayerInfo();
    void InitPlayerUIInfo();
    void UpdatePlayerUIInfo();

    void AddExperienceToPlayer(APlayerController* Player, int32 Amount);

    void StartFarmingMode();


    void EndFarmingMode(); // 0초 됐을 때 처리할 함수
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

public:
    virtual void ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation) override {};
    virtual void TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation) override {};
    virtual void OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation) override {};
    virtual void OnDeathMonster(AActor* Monster, const FVector& DeathLocation) override {};
};
