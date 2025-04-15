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

    int32 StageIndex = 1;

    bool bIsStage1Cleared;
    bool bIsStage2Cleared;
    bool bIsStage3Cleared;

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
    void HandleMonsterKilled(AController* Killer); //���Ͱ� ������ �̰� ȣ��
    
    UFUNCTION(BlueprintCallable)
    void HandleEnemyKilled(AController* Killer); //���Ͱ� ������ �̰� ȣ��

    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
    TSubclassOf<ABaseWitch> DefaultCharacterClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UCooperationWidget> CooperationWidget;


    // ������ ĳ���͸� ������ �迭
    UPROPERTY()
    TArray<ABaseWitch*> SpawnedCharacters;

    // ���Ӹ�� Ŭ������ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
    TSubclassOf<APlayerController> NewPlayerControllerClass;


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> PlayerSpawnLocations;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> PlayerSettingLocations;

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

    TMap<FVector, AActor*> ActiveMonsters;

    void SpawnMonsters();

    int32 CurrentMonsterCount = 0;
    const int32 MaxMonsterCount = 6;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<AActor> MonsterBlueprintClass;


    // Stage2
    //Enemy ������ ���� �ֱ�
    //UPROPERTY(BlueprintReadWrite, Category = "Spawn")
    //TSubclassOf<AEnemySpawner> EnemySpawner;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> EnemySpawnLocations;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<AActor> EnemyBlueprintClass;

    TMap<FVector, AActor*> ActiveEnemies;

    void SpawnEnemies();

    int32 CurrentEnemyCount = 0;
    const int32 MaxEnemyCount = 4;

    // Stage3
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> BossSpawnLocations;




protected:
    void InitPlayerUI();

    //��Ƽ ����
    virtual void PostSeamlessTravel() override;
};
