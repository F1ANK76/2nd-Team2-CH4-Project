// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "../Player/BaseWitch.h"
#include "../Widget/LevelWidget/CooperationWidget.h"
#include "GameState/CooperationGameState.h"
#include "CooperationGameMode.generated.h"


UCLASS()
class ORIGINALSINPRJ_API ACooperationGameMode : public AGameMode
{
	GENERATED_BODY()

    //GameMode Default Function
public:
    ACooperationGameMode();

    virtual void StartPlay() override; // BeginPlay���� ���� ȣ��
    virtual void BeginPlay() override; // ������ ������ �غ� �Ǹ� ȣ��

    //Added GameMode Function
    //Control Game Function
    UFUNCTION(BlueprintCallable)
    void StartGame(); //game ���� Ʈ����
    void EndGame();
    
public:
    TObjectPtr<ACooperationGameState> CooperationGameState = nullptr;


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
    void ReadyStage1(); //Stage1 ���� Ʈ����

    UFUNCTION(BlueprintCallable)
    void ReadyStage2(); //Stage2 ���� Ʈ����

    UFUNCTION(BlueprintCallable)
    void ReadyStage3(); //Stage3 ���� Ʈ����

    UFUNCTION(BlueprintCallable)
    void StartStage1(); //Stage1 ���� Ʈ����
    
    UFUNCTION(BlueprintCallable)
    void StartStage2(); //Stage2 ���� Ʈ����
    
    UFUNCTION(BlueprintCallable)
    void StartStage3(); //Stage3 ���� Ʈ����
    
    UFUNCTION(BlueprintCallable)
    void EndStage1(); //Stage1 ���� Ʈ����
    
    UFUNCTION(BlueprintCallable)
    void EndStage2(); //Stage2 ���� Ʈ����
    
    UFUNCTION(BlueprintCallable)
    void EndStage3(); //Stage3 ���� Ʈ����
    
    //�������� ���۸��� �÷��̾� ��ġ ������ ���� �����ϱ�
    void SetPlayerLocation();

    //�� Stage ���� �� �Ų����� ����� ��ȯ�ϱ� ���� �Լ� �ʿ�
    void MoveNextStage();

    void RequestTurnOnBuffSelectUI();

    void ApplyBuffToBothPlayer();

    UFUNCTION(BlueprintCallable)
    void HandleMonsterKilled(AActor* DeadMonster, AController* Killer); //���Ͱ� ������ �̰� ȣ��
    
    UFUNCTION(BlueprintCallable)
    void HandleEnemyKilled(AActor* DeadMonster, AController* Killer); //���Ͱ� ������ �̰� ȣ��

    UFUNCTION(BlueprintCallable)
    void HandlePlayerKilled(AActor* DeadPlayer, AController* Killer); //�÷��̾ ������ �̰� ȣ��


    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
    TSubclassOf<ABaseWitch> DefaultCharacterClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UCooperationWidget> CooperationWidget;


    // ������ ĳ���͸� ������ �迭
    UPROPERTY(BlueprintReadWrite)
    TArray<ABaseWitch*> SpawnedCharacters;

    TArray<AActor*> ActivePlayers;

    int CurrentPlayerCount = 0;


    // ���Ӹ�� Ŭ������ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
    TSubclassOf<APlayerController> NewPlayerControllerClass;


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> PlayerSpawnLocations;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> PlayerSettingLocations;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> PlayerResultLocations;

    void RequestOpenResultUI();


    void CheckUntilAllPlayerSelectBuff(); // ��� �÷��̾ ������ �����ߴ��� ���� ����ϴ� �Լ�.


    // ĳ���� ���� �Լ�
    void SpawnPlayers();

    void PossessCharacter(APlayerController* PC, APawn* PawnToPossess);

    void HandleClientPossession(APlayerController* PC, int index);

    TSubclassOf<APlayerController> PlayerControllerClass;

    void SetPlayerUnReady(ACharacter* PlayerChar);

    void SetPlayerReady(ACharacter* PlayerChar);


    // Stage1
    //���� ������ ���� �ֱ�
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
    //Enemy ������ ���� �ֱ�
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


    //�÷��̾� �����̵�(ĳ����, �����̼�)
    UFUNCTION(BlueprintCallable)
    void BossSetPlayerLocation(ACharacter* PlayerChar);


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ForceMove")
    TArray<FVector> BossHijackingLocation;

    FVector PlayerHijackedLocation = FVector::ZeroVector;

    UFUNCTION(BlueprintCallable)
    void BossReturnPlayerLocation(ACharacter* PlayerChar);
    
        
        //�Ǵٸ� ���� ��ȯ?
    UFUNCTION(BlueprintCallable)
    void SpawnGhostBoss();

    UFUNCTION(BlueprintCallable)
    void HandleBossMonsterKilled(AController* Killer);

protected:
    void InitPlayerUI();

    //��Ƽ ����
    virtual void PostSeamlessTravel() override;
};
