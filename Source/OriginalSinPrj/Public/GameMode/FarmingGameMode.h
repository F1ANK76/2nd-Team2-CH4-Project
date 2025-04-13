// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "OriginalSinPrj/Interface/BattleEvent.h"
#include "OriginalSinPrj/Interface/MatchManage.h"
#include "../Player/BaseWitch.h"
#include "GameState/FarmingGameState.h"
#include "../Widget/LevelWidget/BattleWidget.h"
#include "FarmingGameMode.generated.h"

UCLASS()
class ORIGINALSINPRJ_API AFarmingGameMode : public AGameMode, public IBattleEvent, public IMatchManage
{
	GENERATED_BODY()
	

    //GameMode Default Function
public:
    AFarmingGameMode();
    virtual void StartPlay() override; // BeginPlay보다 먼저 호출
    virtual void BeginPlay() override; // 게임을 시작할 준비가 되면 호출
    TObjectPtr<AFarmingGameState> FarmingGameState = nullptr;
    //Added GameMode Function
    //Control Game Function
public:
    UFUNCTION(BlueprintCallable)
    void StartGame(); // 파밍모드 시작

    void EndGame(); // 타이머 = 0 이면 호출

    UFUNCTION(BlueprintCallable)
    void HandleMonsterKilled(AController* Killer); //몬스터가 죽으면 이걸 호출
    
    void HandleFarmingModeEnded(); // 파밍 모드 끝내고 다음 레벨로 넘어간다던가....


    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
    TSubclassOf<ABaseWitch> DefaultCharacterClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UBattleWidget> BattleWidgetClass;

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

    void SpawnPlayerByCharacterType(UClass* SpawnClass, APlayerController* PlayerController);


    //몬스터 스포너 갖고 있기
    //UPROPERTY(BlueprintReadWrite, Category = "Spawn")
    //TSubclassOf<AMonsterSpawner> MonsterSpawner;


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> MonsterSpawnLocations;

    TMap<FVector, AActor*> ActiveMonsters;


protected:
    void SpawnInitialMonsters();
    void SpawnMissingMonsters();

    int32 CurrentMonsterCount = 0;
    const int32 MaxMonsterCount = 6;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<AActor> MonsterBlueprintClass;


    ENetMode NetMode;

    void InitPlayerUI();

    //싱글 전용
    void PostLogin(APlayerController* NewPlayer); // 처음 플레이어가 로그인 하면... 테스트용...

    //멀티 전용

    virtual void PostSeamlessTravel() override;

    
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
