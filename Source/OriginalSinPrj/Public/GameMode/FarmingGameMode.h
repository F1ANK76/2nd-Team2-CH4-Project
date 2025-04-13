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
    virtual void StartPlay() override; // BeginPlay���� ���� ȣ��
    virtual void BeginPlay() override; // ������ ������ �غ� �Ǹ� ȣ��
    TObjectPtr<AFarmingGameState> FarmingGameState = nullptr;
    //Added GameMode Function
    //Control Game Function
public:
    UFUNCTION(BlueprintCallable)
    void StartGame(); // �Ĺָ�� ����

    void EndGame(); // Ÿ�̸� = 0 �̸� ȣ��

    UFUNCTION(BlueprintCallable)
    void HandleMonsterKilled(AController* Killer); //���Ͱ� ������ �̰� ȣ��
    
    void HandleFarmingModeEnded(); // �Ĺ� ��� ������ ���� ������ �Ѿ�ٴ���....


    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
    TSubclassOf<ABaseWitch> DefaultCharacterClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UBattleWidget> BattleWidgetClass;

    // ������ ĳ���͸� ������ �迭
    UPROPERTY()
    TArray<ABaseWitch*> SpawnedCharacters;

    // ���Ӹ�� Ŭ������ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
    TSubclassOf<APlayerController> NewPlayerControllerClass;


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TArray<FVector> PlayerSpawnLocations;

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

    //�̱� ����
    void PostLogin(APlayerController* NewPlayer); // ó�� �÷��̾ �α��� �ϸ�... �׽�Ʈ��...

    //��Ƽ ����

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
