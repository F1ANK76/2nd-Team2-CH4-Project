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

class AKillZone;

UCLASS()
class ORIGINALSINPRJ_API AFarmingGameMode : public AGameMode, public IBattleEvent
{
	GENERATED_BODY()
	
    //낙사 있음, 서로 데미지 없음.
    //GameMode Default Function
public:
    AFarmingGameMode();
    virtual void StartPlay() override; // BeginPlay보다 먼저 호출
    virtual void BeginPlay() override; // 게임을 시작할 준비가 되면 호출

    TObjectPtr<AFarmingGameState> FarmingGameState = nullptr;
    
    void MoveLevel(const FName& LevelName);
    
    void StartSingleGame();
    
    void StartMultiGame();
    
    void EndSingleGame();
    
    void EndMultiGame();
    
    void EndGame(); // 타이머 = 0 이면 호출

    UFUNCTION()
    void OnCharacterStateReceived(const FCharacterStateBuffer& State);

    //Added GameMode Function
    //Control Game Function
public:
    UPROPERTY(EditDefaultsOnly, Category = "Camera")
    TSubclassOf<ABaseCamera> BaseCamera;

    // 생성된 카메라를 관리할 배열
    UPROPERTY()
    TArray<ABaseCamera*> SpawnedBaseCamera;

    //카메라 생성 함수
    void SpawnCamera();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BossCamera")
    TArray<FVector> CameraSpawnLocations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KillZone")
    TSubclassOf<AKillZone> ActorKillZone;

    // killzone 생성 함수
    void SpawnKillZone();



    void RequestTurnOnPlayerUI();
    void RequestTurnOnEnemyUI();
    

    UFUNCTION(BlueprintCallable)
    void HandleMonsterKilled(AActor* DeadMonster, AActor* Killer); //몬스터가 죽으면 이걸 호출


    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
    TSubclassOf<ABaseWitch> DefaultCharacterClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UBattleWidget> BattleWidgetClass;

    // 생성된 캐릭터를 관리할 배열
    UPROPERTY()
    TArray<ABaseWitch*> SpawnedCharacters;

    UPROPERTY()
    TArray<AActor*> AlivePlayers;
    TArray<AActor*> DeadPlayers;

    TArray<AActor*> ActivePlayers;


    void RequestUpdateUI(int PlayerIndex)
    {
        FarmingGameState->UpdatePlayerUIInfo();
        if (PlayerIndex == 0)
        {
            FarmingGameState->Player1DataChanged++;
        }

        if (PlayerIndex == 1)
        {
            FarmingGameState->Player2DataChanged++;
        }
    }



    void AddCharacterOnAlivePlayers(AActor* Actor);

    void SetPlayerLocation(AActor* Player);

    // 게임모드 클래스에 선언
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
    TSubclassOf<APlayerController> NewPlayerControllerClass;


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> PlayerSpawnLocations;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> PlayerReSpawnLocations;

    void PlayerRespawn();


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

    TArray<AActor*> ActiveMonsters;


    void AttachPlayerToCamera(ACharacter* Player, ABaseCamera* Camera);


    void SpawnInitialMonsters();
    void SpawnMissingMonsters();

    int32 CurrentMonsterCount = 0;
    const int32 MaxMonsterCount = 6;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<AActor> MonsterBlueprintClass;

    ENetMode NetMode;

    void InitPlayerUI();

    //Test
    void PostLogin(APlayerController* NewPlayer); // 처음 플레이어가 로그인 하면... 테스트용...

    //멀티 전용

    virtual void PostSeamlessTravel() override;

  

public:
    virtual void ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation) override;
    virtual void TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation) override;
    virtual void OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation) override;
    virtual void OnDeathMonster(AActor* Monster, const FVector& DeathLocation) override;   
};
