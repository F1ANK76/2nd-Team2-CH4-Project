// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "../Player/BaseWitch.h"
#include "../Widget/LevelWidget/CooperationWidget.h"
#include "GameState/CooperationGameState.h"
#include "BaseCamera.h"
#include "CooperationGameMode.generated.h"


UCLASS()
class ORIGINALSINPRJ_API ACooperationGameMode : public AGameMode, public IBattleEvent
{
	GENERATED_BODY()

    //GameMode Default Function
public: //for test
    UFUNCTION()
    void HandleBuffSelection(AActor* SourceActor, int32 BuffIndex);

    void ApplyBuffToPlayer(APlayerController* Controller, int32 BuffIndex, FBuffInfo buff);
    
    void RequestTurnOffBuffSelectUI();
    
    UFUNCTION()
    void OnCharacterStateReceived(const FCharacterStateBuffer& State);

public:
    ACooperationGameMode();
    virtual void StartPlay() override; // BeginPlay보다 먼저 호출
    virtual void BeginPlay() override; // 게임을 시작할 준비가 되면 호출

    //Added GameMode Function
    //Control Game Function
    UFUNCTION(BlueprintCallable)
    void StartGame(); //game 시작 트리거

    void EndGame();


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
    }

    
public:
    TObjectPtr<ACooperationGameState> CooperationGameState = nullptr;


    UPROPERTY(BlueprintReadWrite)
    TArray<ABaseWitch*> SpawnedCharacters;
    
    // 생성된 캐릭터를 관리할 배열
    TArray<AActor*> ActivePlayers;

    // return Current Activated Players
    TArray<AActor*> GetActivePlayers() const
    {
        return ActivePlayers;
    }

    int CurrentPlayerCount = 0;


    // 게임모드 클래스에 선언
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

    // 생성된 카메라를 관리할 배열
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

    //카메라 생성 함수
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
    void ReadyStage1(); //Stage1 세팅 트리거

    UFUNCTION(BlueprintCallable)
    void ReadyStage2(); //Stage2 세팅 트리거

    UFUNCTION(BlueprintCallable)
    void ReadyStage3(); //Stage3 세팅 트리거

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
    
    //스테이지 시작마다 플레이어 위치 정해진 곳에 조정하기
    void SetPlayerLocation();

    //각 Stage 종료 후 매끄럽게 장면을 전환하기 위한 함수 필요
    void MoveNextStage();

    void RequestTurnOnBuffSelectUI();

    void ApplyBuffToBothPlayer();

    void PlayerDie(AActor* DeadPlayer, AActor* Killer);

    void Respawn(AActor* DeadActor);

    UFUNCTION(BlueprintCallable)
    void HandleMonsterKilled(AActor* DeadMonster, AActor* Killer); //몬스터가 죽으면 이걸 호출
    
    UFUNCTION(BlueprintCallable)
    void HandleEnemyKilled(AActor* DeadMonster, AActor* Killer); //몬스터가 죽으면 이걸 호출

    UFUNCTION(BlueprintCallable)
    void HandlePlayerKilled(AActor* DeadPlayer, AActor* Killer); //플레이어가 죽으면 이걸 호출

    //낙사처리

    UFUNCTION()
    void FallDie(AActor* Character);


    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
    TSubclassOf<ABaseWitch> DefaultCharacterClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UCooperationWidget> CooperationWidget;



    void RequestOpenResultUI();

    void CheckUntilAllPlayerSelectBuff(); // 모든 플레이어가 버프를 선택했는지 보고 대기하는 함수.


    // 캐릭터 생성 함수
    void SpawnPlayers();

    void PossessCharacter(APlayerController* PC, APawn* PawnToPossess);

    void HandleClientPossession(APlayerController* PC, int index);

    TSubclassOf<APlayerController> PlayerControllerClass;

    void SetPlayerUnReady();

    void SetPlayerReady();


    // Stage1
    //몬스터 스포너 갖고 있기
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
    //Enemy 스포너 갖고 있기
    //UPROPERTY(BlueprintReadWrite, Category = "Spawn")
    //TSubclassOf<AEnemySpawner> EnemySpawner;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> EnemySpawnLocations;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<AActor> EnemyBlueprintClass;

    UPROPERTY(BlueprintReadWrite)
    TArray<AActor*> ActiveEnemies;

    void SpawnEnemies();

    int32 CurrentEnemyCount = 0;
    const int32 MaxEnemyCount = 4;

    // Stage3
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> BossSpawnLocations;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<AActor> BossBlueprintClass;

    void SpawnBossMonsters();


    UPROPERTY(BlueprintReadWrite)
    TArray<AActor*> ActiveBossMonster;

    //스테이트에서 0이면 -> end battle 호출 -> 보스 내부적으로 로직 해제 -> 사망모션은 보스쪽에서. -> 게임모드는 그대로....

    TArray<AActor*> StartBattle(TArray<AActor*> Players);



    //플레이어 강제이동(캐릭터, 로케이션)
    UFUNCTION(BlueprintCallable)
    void BossSetPlayerLocation(ACharacter* PlayerChar);


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForceMove")
    TArray<FVector> BossHijackingLocation;

    FVector PlayerHijackedLocation = FVector::ZeroVector;

    UFUNCTION(BlueprintCallable)
    void BossReturnPlayerLocation(ACharacter* PlayerChar);
    
        
        //또다른 보스 소환?
    UFUNCTION(BlueprintCallable)
    void SpawnGhostBoss();

    UFUNCTION(BlueprintCallable)
    void HandleBossMonsterKilled(AActor* Killer);

    UFUNCTION()
    void TravelLevel();

protected:
    //UFUNCTION(NetMulticast, Reliable)
    void InitPlayerUI();

    //멀티 전용
    virtual void PostSeamlessTravel() override;

public:
    virtual void ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation) override;
    virtual void TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation) override;
    virtual void OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation) override;
    virtual void OnDeathMonster(AActor* Monster, const FVector& DeathLocation) override;
};