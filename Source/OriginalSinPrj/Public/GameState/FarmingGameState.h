// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OriginalSinPrj/Interface/BattleEvent.h"
#include "OriginalSinPrj/Interface/MatchManage.h"
#include "../Widget/LevelWidget/BattleWidget.h"
#include "../Widget/AddedWidget/PlayerStateWidget.h"
#include "../Player/BaseWitch.h"
#include "GameFramework/GameState.h"
#include "FarmingGameState.generated.h"



UCLASS()
class ORIGINALSINPRJ_API AFarmingGameState : public AGameState, public IBattleEvent, public IMatchManage
{
	GENERATED_BODY()
	
    
public:
    AFarmingGameState();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;


    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(Replicated)
    bool bIsFarmingStarted;

    UPROPERTY(Replicated, BlueprintReadOnly)
    float TimeRemaining;

    // Player 정보 관리
    UPROPERTY(BlueprintReadOnly)
    TMap<APlayerController*, FPlayerData> PlayerInfos;


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
    
    virtual void FinishMatch() override {};
    virtual void VictoryMatch() override {};
    virtual void DefeatMatch() override {};
    virtual void DrawMatch() override {};
public:
    virtual void ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation) override {};
    virtual void TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation) override {};
    virtual void OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation) override {};
    virtual void OnDeathMonster(AActor* Monster, const FVector& DeathLocation) override {};
};
