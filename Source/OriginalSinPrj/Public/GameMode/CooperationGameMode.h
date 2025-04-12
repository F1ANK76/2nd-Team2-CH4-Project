// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "../Player/BaseWitch.h"
#include "../Widget/LevelWidget/CooperationWidget.h"
#include "CooperationGameMode.generated.h"


UCLASS()
class ORIGINALSINPRJ_API ACooperationGameMode : public AGameMode
{
	GENERATED_BODY()
    //GameMode Default Function
public:
    ACooperationGameMode();

    virtual void StartPlay() override; // BeginPlay보다 먼저 호출
    virtual void BeginPlay() override; // 게임을 시작할 준비가 되면 호출

    //Added GameMode Function
    //Control Game Function
    UFUNCTION(BlueprintCallable)
    void StartGame(); //game 시작 트리거
    void EndGame();
    
public:
    UFUNCTION(BlueprintCallable)
    void StartStage1(); //Stage1 시작 트리거
    
    UFUNCTION(BlueprintCallable)
    void StartStage2(); //Stage2 시작 트리거
    
    UFUNCTION(BlueprintCallable)
    void StartStage3(); //Stage3 시작 트리거
    
    void EndStage1(); //Stage1 종료 트리거
    
    UFUNCTION(BlueprintCallable)
    void EndStage2(); //Stage2 종료 트리거
    
    UFUNCTION(BlueprintCallable)
    void EndStage3(); //Stage3 종료 트리거
    
    //각 Stage 종료 후 매끄럽게 장면을 전환하기 위한 함수 필요
    void TravelNextScene();



    UFUNCTION(BlueprintCallable)
    void HandleMonsterKilled(AController* Killer); //몬스터가 죽으면 이걸 호출

    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
    TSubclassOf<ABaseWitch> DefaultCharacterClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UCooperationWidget> CooperationWidget;


    // 생성된 캐릭터를 관리할 배열
    UPROPERTY()
    TArray<ABaseWitch*> SpawnedCharacters;

    // 게임모드 클래스에 선언
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
    TSubclassOf<APlayerController> NewPlayerControllerClass;


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> PlayerSpawnLocations;

    // 캐릭터 생성 함수
    void SpawnPlayers();

    void PossessCharacter(APlayerController* PC, APawn* PawnToPossess);

    void HandleClientPossession(APlayerController* PC, int index);

    TSubclassOf<APlayerController> PlayerControllerClass;


    // Stage1
    //몬스터 스포너 갖고 있기
    //UPROPERTY(BlueprintReadWrite, Category = "Spawn")
    //TSubclassOf<AMonsterSpawner> MonsterSpawner;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> MonsterSpawnLocations;

    TMap<FVector, AActor*> ActiveMonsters;

    void SpawnInitialMonsters();

    int32 CurrentMonsterCount = 0;
    const int32 MaxMonsterCount = 6;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<AActor> MonsterBlueprintClass;


    // Stage2
    //Enemy 스포너 갖고 있기
    //UPROPERTY(BlueprintReadWrite, Category = "Spawn")
    //TSubclassOf<AEnemySpawner> EnemySpawner;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> EnemySpawnLocations;

    TMap<FVector, AActor*> ActiveEnemies;

    void SpawnEnemies();

    int32 CurrentEnemyCount = 0;
    const int32 MaxEnemyCount = 4;

    // Stage3
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> BossSpawnLocations;




protected:
    void InitPlayerUI();

    //멀티 전용
    virtual void PostSeamlessTravel() override;
};
