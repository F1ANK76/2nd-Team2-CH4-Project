// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "OriginalSinPrj/Interface/BattleEvent.h"
#include "OriginalSinPrj/Interface/MatchManage.h"
#include "../Player/BaseWitch.h"
#include "../Widget/LevelWidget/BattleWidget.h"
#include "FarmingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ORIGINALSINPRJ_API AFarmingGameMode : public AGameMode, public IBattleEvent, public IMatchManage
{
	GENERATED_BODY()
	
public:
    AFarmingGameMode();
    virtual void StartPlay() override; // BeginPlay보다 먼저 호출
    
    virtual void BeginPlay() override; // 게임을 시작할 준비가 되면 호출

    UFUNCTION(BlueprintCallable)
    void StartGame();

    void EndGame();

    UFUNCTION(BlueprintCallable)
    void HandleMonsterKilled(AController* Killer);
    void HandleRoundEnded();

    void PostLogin(APlayerController* NewPlayer);

    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
    TSubclassOf<ABaseWitch> DefaultCharacterClass;

    void SpawnPlayerByCharacterType(UClass* SpawnClass, APlayerController* PlayerController);


    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UBattleWidget> BattleWidgetClass;
    

    // GameMode 헤더
    TSubclassOf<APlayerController> PlayerControllerClass;
    
protected:
    void SpawnInitialMonsters();
    void SpawnMissingMonsters();

    int32 CurrentMonsterCount = 0;
    const int32 MaxMonsterCount = 6;

    // FarmingGameMode.h

    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<AActor> MonsterBlueprintClass;


    
public:
    

    virtual void StartMatch() override {};

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
