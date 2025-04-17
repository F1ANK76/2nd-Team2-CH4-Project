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
	
    //���� ����, ���� ������ ����.
    //GameMode Default Function
public:
    AFarmingGameMode();
    virtual void StartPlay() override; // BeginPlay���� ���� ȣ��
    virtual void BeginPlay() override; // ������ ������ �غ� �Ǹ� ȣ��

    TObjectPtr<AFarmingGameState> FarmingGameState = nullptr;
    
    void MoveLevel();
    
    void StartSingleGame();
    
    void StartMultiGame();
    
    void EndSingleGame();
    
    void EndMultiGame();
    
    void EndGame(); // Ÿ�̸� = 0 �̸� ȣ��

    UFUNCTION()
    void OnCharacterStateReceived(const FCharacterStateBuffer& State);

    //Added GameMode Function
    //Control Game Function
public:
    UPROPERTY(EditDefaultsOnly, Category = "Camera")
    TSubclassOf<ABaseCamera> BaseCamera;

    // ������ ī�޶� ������ �迭
    UPROPERTY()
    TArray<ABaseCamera*> SpawnedBaseCamera;

    //ī�޶� ���� �Լ�
    void SpawnCamera();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BossCamera")
    TArray<FVector> CameraSpawnLocations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KillZone")
    TSubclassOf<AKillZone> ActorKillZone;

    // killzone ���� �Լ�
    void SpawnKillZone();



    void RequestTurnOnPlayerUI();
    void RequestTurnOnEnemyUI();
    

    UFUNCTION(BlueprintCallable)
    void HandleMonsterKilled(AActor* DeadMonster, AActor* Killer); //���Ͱ� ������ �̰� ȣ��


    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
    TSubclassOf<ABaseWitch> DefaultCharacterClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UBattleWidget> BattleWidgetClass;

    // ������ ĳ���͸� ������ �迭
    UPROPERTY()
    TArray<ABaseWitch*> SpawnedCharacters;

    UPROPERTY()
    TArray<AActor*> AlivePlayers;
    TArray<AActor*> DeadPlayers;

    TArray<AActor*> ActivePlayers;

    void SetPlayerUnReady();
    void SetPlayerUnReady(AActor* actor);

    void SetPlayerReady();


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

    // ���Ӹ�� Ŭ������ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
    TSubclassOf<APlayerController> NewPlayerControllerClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> PlayerSpawnLocations;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> PlayerReSpawnLocations;

    void PlayerRespawn();


    // ĳ���� ���� �Լ�
    void SpawnPlayers();

    void PossessCharacter(APlayerController* PC, APawn* PawnToPossess);

    void HandleClientPossession(APlayerController* PC, int index);


    TSubclassOf<APlayerController> PlayerControllerClass;

    void SpawnPlayerByCharacterType(UClass* SpawnClass, APlayerController* PlayerController);


    //���� ������ ���� �ֱ�
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
    void PostLogin(APlayerController* NewPlayer); // ó�� �÷��̾ �α��� �ϸ�... �׽�Ʈ��...

    //��Ƽ ����

    virtual void PostSeamlessTravel() override;

  

public:
    virtual void ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation) override;
    virtual void TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation) override;
    virtual void OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation) override;
    virtual void OnDeathMonster(AActor* Monster, const FVector& DeathLocation) override;   
};
