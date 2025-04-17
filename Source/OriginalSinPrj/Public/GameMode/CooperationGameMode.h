// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "../Player/BaseWitch.h"
#include "../Widget/LevelWidget/CooperationWidget.h"
#include "GameState/CooperationGameState.h"
#include "BaseCamera.h"
#include "KillZone.h"
#include "CooperationGameMode.generated.h"


UCLASS()
class ORIGINALSINPRJ_API ACooperationGameMode : public AGameMode, public IBattleEvent
{
    GENERATED_BODY()

    //GameMode Default Function
public: //for test
    UFUNCTION()
    void HandleBuffSelection(AActor* SourceActor, int32 BuffIndex);

    void ApplyBuffToPlayer(APlayerController* Controller, int32 BuffIndex, EBuffType buff);

    void RequestTurnOffBuffSelectUI();

    UFUNCTION()
    void OnCharacterStateReceived(const FCharacterStateBuffer& State);

    UFUNCTION()
    void OnEnemyStateReceived(const FCharacterStateBuffer& State);

public:
    ACooperationGameMode();
    virtual void StartPlay() override; // BeginPlayï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ È£ï¿½ï¿½
    virtual void BeginPlay() override; // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Øºï¿½ ï¿½Ç¸ï¿½ È£ï¿½ï¿½

    //Added GameMode Function
    //Control Game Function
    UFUNCTION(BlueprintCallable)
    void StartGame(); //game ï¿½ï¿½ï¿½ï¿½ Æ®ï¿½ï¿½ï¿½ï¿½

    void EndGame();
    void SeamlessTravel();


    void RequestUpdateUI(int PlayerIndex)
    {
        CooperationGameState->UpdatePlayerUIInfo();
        if (PlayerIndex == 0)
        {
            CooperationGameState->Player1DataChanged++;
        }

        if (PlayerIndex == 1)
        {
            CooperationGameState->Player2DataChanged++;
        }

        if (PlayerIndex == 2)
        {
            CooperationGameState->Enemy1DataChanged++;
        }
    }


public:
    TObjectPtr<ACooperationGameState> CooperationGameState = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KillZone")
    TSubclassOf<AKillZone> ActorKillZone;

    void SpawnKillZone();

    UPROPERTY(BlueprintReadWrite)
    TArray<ABaseWitch*> SpawnedCharacters;

    TArray<AActor*> ActivePlayers;
    TArray<AActor*> AlivePlayers;

    void ResetAlivePlayers();



    // return Current Activated Players
    TArray<AActor*> GetActivePlayers() const
    {
        return ActivePlayers;
    }

    int CurrentPlayerCount = 0;


    // ï¿½ï¿½ï¿½Ó¸ï¿½ï¿?Å¬ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
    TSubclassOf<APlayerController> NewPlayerControllerClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> PlayerSpawnLocations;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> PlayerSettingLocations;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> PlayerResultLocations;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector>RespawnLocation;


    UPROPERTY(EditDefaultsOnly, Category = "Camera")
    TSubclassOf<ABaseCamera> BaseCamera;

    // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ Ä«ï¿½Þ¶ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½è¿­
    UPROPERTY()
    TArray<ABaseCamera*> SpawnedBaseCamera;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BossCamera")
    TArray<FVector> CameraSpawnLocations;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BossCamera")
    TArray<FVector> BossStageCameraLocations;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BossCamera")
    TArray<float> BossStageCameraDistance;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BossCamera")
    TArray<FRotator> BossStageCameraAngle;

    //Ä«ï¿½Þ¶ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ô¼ï¿½
    void SpawnCamera();

    void AttachPlayerToCamera(ACharacter* Player, ABaseCamera* Camera);

    UPROPERTY(BlueprintReadWrite)
    int32 StageIndex = 1;

    int Player1ColorIndex = 0;
    int Player2ColorIndex = 1;

    void SetPlayerColorIndex();

    UFUNCTION(BlueprintCallable)
    int GetPlayerColorIndex(ACharacter* PlayerChar);

    int GetPlayerColorIndex(AController* PlayController);

    bool bIsStage1Cleared = false;
    bool bIsStage2Cleared = false;
    bool bIsStage3Cleared = false;

    UFUNCTION(BlueprintCallable)
    void ReadyStage1(); //Stage1 ï¿½ï¿½ï¿½ï¿½ Æ®ï¿½ï¿½ï¿½ï¿½

    UFUNCTION(BlueprintCallable)
    void ReadyStage2(); //Stage2 ï¿½ï¿½ï¿½ï¿½ Æ®ï¿½ï¿½ï¿½ï¿½

    UFUNCTION(BlueprintCallable)
    void ReadyStage3(); //Stage3 ï¿½ï¿½ï¿½ï¿½ Æ®ï¿½ï¿½ï¿½ï¿½

    UFUNCTION(BlueprintCallable)
    void StartStage1(); //Stage1 ½ÃÀÛ Æ®¸®°Å

    UFUNCTION(BlueprintCallable)
    void StartStage2(); //Stage2 ½ÃÀÛ Æ®¸®°Å

    UFUNCTION(BlueprintCallable)
    void StartStage3(); //Stage3 ½ÃÀÛ Æ®¸®°Å

    UFUNCTION(BlueprintCallable)
    void EndStage1(); //Stage1 Á¾·á Æ®¸®°Å

    UFUNCTION(BlueprintCallable)
    void EndStage2(); //Stage2 Á¾·á Æ®¸®°Å

    UFUNCTION(BlueprintCallable)
    void EndStage3(); //Stage3 Á¾·á Æ®¸®°Å



    void SetPlayerLocation();

    //ï¿½ï¿½ Stage ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ ï¿½Å²ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½ï¿½ï¿½ï¿?ï¿½ï¿½È¯ï¿½Ï±ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ô¼ï¿½ ï¿½Ê¿ï¿½
    void MoveNextStage();

    void RequestTurnOnBuffSelectUI();

    void ApplyBuffToBothPlayer();

    void PlayerDie(AActor* DeadPlayer, AActor* Killer);
    void PlayerFallDie(AActor* DeadPlayer, AActor* Killer);

    void Respawn(AActor* DeadActor);

    UFUNCTION(BlueprintCallable)
    void HandleMonsterKilled(AActor* DeadMonster, AActor* Killer); //¸ó½ºÅÍ°¡ Á×À¸¸é ÀÌ°É È£Ãâ


    UFUNCTION(BlueprintCallable)
    void HandleEnemyKilled(AActor* DeadMonster, AActor* Killer); //ï¿½ï¿½ï¿½Í°ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ì°ï¿½ È£ï¿½ï¿½

    UFUNCTION(BlueprintCallable)
    void HandlePlayerKilled(AActor* DeadPlayer, AActor* Killer); //ï¿½Ã·ï¿½ï¿½Ì¾î°¡ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ì°ï¿½ È£ï¿½ï¿½

    //ï¿½ï¿½ï¿½ï¿½Ã³ï¿½ï¿½

    UFUNCTION()
    void FallDie(AActor* Character);


    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
    TSubclassOf<ABaseWitch> DefaultCharacterClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UCooperationWidget> CooperationWidget;

    bool bIsClear = true;

    void RequestOpenResultUI();

    void CheckUntilAllPlayerSelectBuff(); // ï¿½ï¿½ï¿?ï¿½Ã·ï¿½ï¿½Ì¾î°¡ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ß´ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½Ï´ï¿?ï¿½Ô¼ï¿½.


    // Ä³ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ô¼ï¿½
    void SpawnPlayers();

    void PossessCharacter(APlayerController* PC, APawn* PawnToPossess);

    void HandleClientPossession(APlayerController* PC, int index);

    TSubclassOf<APlayerController> PlayerControllerClass;

    void SetPlayerUnReady();
    void SetPlayerUnReady(AActor* actor);

    void SetPlayerReady();


    // Stage1
    //ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ö±ï¿½
    //UPROPERTY(BlueprintReadWrite, Category = "Spawn")
    //TSubclassOf<AMonsterSpawner> MonsterSpawner;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> MonsterSpawnLocations;

    TArray<AActor*> ActiveMonsters;




    void SpawnMonsters();

    int32 CurrentMonsterCount = 0;
    const int32 MaxMonsterCount = 6;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<AActor> MonsterBlueprintClass;

    UPROPERTY(EditAnywhere, Category = "StageClearTrigger")
    TArray<UObject*> Stage1ClearTriggerObject;

    UFUNCTION(BlueprintCallable)
    void TriggerStage1Clear(UObject* Object);

    bool Stage1ClearTrigger1 = false;
    bool Stage1ClearTrigger2 = false;


    // Stage2
    //Enemy ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ö±ï¿½
    //UPROPERTY(BlueprintReadWrite, Category = "Spawn")
    //TSubclassOf<AEnemySpawner> EnemySpawner;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> EnemySpawnLocations;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<AActor> EnemyBlueprintClass;

    UPROPERTY(BlueprintReadWrite)
    TArray<AActor*> ActiveEnemies;

    TArray<AActor*> AliveEnemies;

    void SpawnEnemies();

    int32 CurrentEnemyCount = 0;
    const int32 MaxEnemyCount = 4;

    // Stage3
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> BossSpawnLocations;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<AActor> BossBlueprintClass;

    void SpawnBossMonsters();

    void InitEnemyInfo();
    void InitEnemyUIInfo();

    UPROPERTY(BlueprintReadWrite)
    TArray<AActor*> ActiveBossMonster;

    //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ®ï¿½ï¿½ï¿½ï¿½ 0ï¿½Ì¸ï¿½ -> end battle È£ï¿½ï¿½ -> ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ -> ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ê¿ï¿½ï¿½ï¿½. -> ï¿½ï¿½ï¿½Ó¸ï¿½ï¿½ï¿½ ï¿½×´ï¿½ï¿?...

    TArray<AActor*> StartBattle(TArray<AActor*> Players);



    //ï¿½Ã·ï¿½ï¿½Ì¾ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ìµï¿½(Ä³ï¿½ï¿½ï¿½ï¿½, ï¿½ï¿½ï¿½ï¿½ï¿½Ì¼ï¿½)
    UFUNCTION(BlueprintCallable)
    void BossSetPlayerLocation(ACharacter* PlayerChar);


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForceMove")
    TArray<FVector> BossHijackingLocation;

    FVector PlayerHijackedLocation = FVector::ZeroVector;

    UFUNCTION(BlueprintCallable)
    void BossReturnPlayerLocation(ACharacter* PlayerChar);

    UFUNCTION(BlueprintCallable)
    void SpawnGhostBoss();

    UFUNCTION(BlueprintCallable)
    void HandleBossMonsterKilled(AActor* Killer);

    UFUNCTION()
    void TravelLevel();




protected:
    //UFUNCTION(NetMulticast, Reliable)
    void InitPlayerUI();

    //ï¿½ï¿½Æ¼ ï¿½ï¿½ï¿½ï¿½
    virtual void PostSeamlessTravel() override;


public:
    virtual void ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation) override;
    virtual void TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation) override;
    virtual void OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation) override;
    virtual void OnDeathMonster(AActor* Monster, const FVector& DeathLocation) override;
};