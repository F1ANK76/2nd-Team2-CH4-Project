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
    virtual void StartPlay() override; // BeginPlay占쏙옙占쏙옙 占쏙옙占쏙옙 호占쏙옙
    virtual void BeginPlay() override; // 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쌔븝옙 占실몌옙 호占쏙옙

    //Added GameMode Function
    //Control Game Function
    UFUNCTION(BlueprintCallable)
    void StartGame(); //game 占쏙옙占쏙옙 트占쏙옙占쏙옙

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


    // 占쏙옙占쌈몌옙占?클占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
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

    // 占쏙옙占쏙옙占쏙옙 카占쌨띰옙 占쏙옙占쏙옙占쏙옙 占썼열
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

    //카占쌨띰옙 占쏙옙占쏙옙 占쌉쇽옙
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
    void ReadyStage1(); //Stage1 占쏙옙占쏙옙 트占쏙옙占쏙옙

    UFUNCTION(BlueprintCallable)
    void ReadyStage2(); //Stage2 占쏙옙占쏙옙 트占쏙옙占쏙옙

    UFUNCTION(BlueprintCallable)
    void ReadyStage3(); //Stage3 占쏙옙占쏙옙 트占쏙옙占쏙옙

    UFUNCTION(BlueprintCallable)
    void StartStage1(); //Stage1 시작 트리거

    UFUNCTION(BlueprintCallable)
    void StartStage2(); //Stage2 시작 트리거

    UFUNCTION(BlueprintCallable)
    void StartStage3(); //Stage3 시작 트리거

    UFUNCTION(BlueprintCallable)
    void EndStage1(); //Stage1 종료 트리거

    UFUNCTION(BlueprintCallable)
    void EndStage2(); //Stage2 종료 트리거

    UFUNCTION(BlueprintCallable)
    void EndStage3(); //Stage3 종료 트리거



    void SetPlayerLocation();

    //�� Stage ���� �� �Ų���� ����� ��ȯ�ϱ� ���� �Լ� �ʿ�
    void MoveNextStage();

    void RequestTurnOnBuffSelectUI();

    void ApplyBuffToBothPlayer();

    void PlayerDie(AActor* DeadPlayer, AActor* Killer);
    void PlayerFallDie(AActor* DeadPlayer, AActor* Killer);

    void Respawn(AActor* DeadActor);

    UFUNCTION(BlueprintCallable)
    void HandleMonsterKilled(AActor* DeadMonster, AActor* Killer); //몬스터가 죽으면 이걸 호출


    UFUNCTION(BlueprintCallable)
    void HandleEnemyKilled(AActor* DeadMonster, AActor* Killer); //占쏙옙占싶곤옙 占쏙옙占쏙옙占쏙옙 占싱곤옙 호占쏙옙

    UFUNCTION(BlueprintCallable)
    void HandlePlayerKilled(AActor* DeadPlayer, AActor* Killer); //占시뤄옙占싱어가 占쏙옙占쏙옙占쏙옙 占싱곤옙 호占쏙옙

    //占쏙옙占쏙옙처占쏙옙

    UFUNCTION()
    void FallDie(AActor* Character);


    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
    TSubclassOf<ABaseWitch> DefaultCharacterClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UCooperationWidget> CooperationWidget;

    bool bIsClear = true;

    void RequestOpenResultUI();

    void CheckUntilAllPlayerSelectBuff(); // 占쏙옙占?占시뤄옙占싱어가 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쌩댐옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙求占?占쌉쇽옙.


    // 캐占쏙옙占쏙옙 占쏙옙占쏙옙 占쌉쇽옙
    void SpawnPlayers();

    void PossessCharacter(APlayerController* PC, APawn* PawnToPossess);

    void HandleClientPossession(APlayerController* PC, int index);

    TSubclassOf<APlayerController> PlayerControllerClass;

    void SetPlayerUnReady();
    void SetPlayerUnReady(AActor* actor);

    void SetPlayerReady();


    // Stage1
    //占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쌍깍옙
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
    //Enemy 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쌍깍옙
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

    //占쏙옙占쏙옙占쏙옙트占쏙옙占쏙옙 0占싱몌옙 -> end battle 호占쏙옙 -> 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙 -> 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占십울옙占쏙옙. -> 占쏙옙占쌈몌옙占쏙옙 占쌓댐옙占?...

    TArray<AActor*> StartBattle(TArray<AActor*> Players);



    //占시뤄옙占싱억옙 占쏙옙占쏙옙占싱듸옙(캐占쏙옙占쏙옙, 占쏙옙占쏙옙占싱쇽옙)
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

    //��Ƽ ����
    virtual void PostSeamlessTravel() override;

public:
    virtual void ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation) override;
    virtual void TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation) override;
    virtual void OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation) override;
    virtual void OnDeathMonster(AActor* Monster, const FVector& DeathLocation) override;
};