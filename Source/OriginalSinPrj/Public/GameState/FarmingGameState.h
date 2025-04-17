// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OriginalSinPrj/Interface/BattleEvent.h"
#include "../Widget/LevelWidget/BattleWidget.h"
#include "../Widget/AddedWidget/PlayerStateWidget.h"
#include "../Player/BaseWitch.h"
#include "OriginalSinPrj/GameInstance/OriginalSinPrjGameInstance.h"
#include "GameFramework/GameState.h"
#include "BaseCamera.h"
#include "OriginalSinPrj/Interface/CameraStateInterface.h"
#include "FarmingGameState.generated.h"

class AFarmingGameMode;
class AKillZone;

UCLASS()
class ORIGINALSINPRJ_API AFarmingGameState : public AGameState, public IBattleEvent, public ICameraStateInterface
{
	GENERATED_BODY()
	
    
public:
    AFarmingGameState();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    AFarmingGameMode* FarmingGameMode;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


    void SetPlayerMove(bool bCanMove);

    UPROPERTY(ReplicatedUsing = OnRep_SetPlayerMove)
    bool bIsPlayerCanMove;

    UFUNCTION()
    void OnRep_SetPlayerMove();



    void SetMyDataForNextLevel(int32 Level);


    //ī�޶� ó���� �Լ�.
    virtual FVector GetCameraLocation() const override { return CameraLocation; }
    virtual FRotator GetCameraRotation() const override { return CameraRotation; }
    virtual float GetCameraDistance() const override { return CameraDistance; }


    void SetCameraTransform();


    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Camera")
    FVector CameraLocation;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Camera")
    FRotator CameraRotation;

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "Camera")
    float CameraDistance;



    UPROPERTY(BlueprintReadOnly)
    TMap<AActor*, FPlayerData> PlayerInfos;


    UPROPERTY(Replicated)
    FPlayerData Player1StateData;

    UPROPERTY(Replicated)
    FPlayerData Player2StateData;


    UPROPERTY(Replicated)
    bool bIsFarmingStarted;

    UPROPERTY(ReplicatedUsing = OnRep_UpdateTimer)
    float TimeRemaining;

    UFUNCTION()
    void OnRep_UpdateTimer();

    UPROPERTY(ReplicatedUsing = OnRep_SaveLevel)
    int SaveTrigger = 0;

    UFUNCTION()
    void OnRep_SaveLevel();

    void SaveLevel();


    void UpdateTimer();


    UPROPERTY(ReplicatedUsing = OnRep_UpdatePlayer1DataUI)
    int Player1DataChanged = 0;

    UFUNCTION()
    void OnRep_UpdatePlayer1DataUI();

    UPROPERTY(ReplicatedUsing = OnRep_UpdatePlayer2DataUI)
    int Player2DataChanged = 0;

    UFUNCTION()
    void OnRep_UpdatePlayer2DataUI();



    void TurnOnPlayerUI();
    void TurnOnAllUI();

    UPROPERTY(ReplicatedUsing = OnRep_TurnOnAllUI)
    int MultiPlayer = 0;
    
    UFUNCTION()
    void OnRep_TurnOnAllUI();

    void InitPlayerInfo();
    void UpdatePlayerInfo(const FCharacterStateBuffer& State);
    void InitPlayerUIInfo();
    void UpdatePlayerUIInfo();

    void AddExperienceToPlayer(APlayerController* Player, int32 Amount);

    void StartFarmingMode();

    void EndFarmingMode(); // 0�� ���� �� ó���� �Լ�
private:

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UBattleWidget> BattleWidgetClass; // UI Ŭ����
    

    FTimerHandle TimerHandle;
    void CheckLevelUp(APlayerController* Player);
    
public:
    void SetPlayerPawn(ABaseWitch* InPawn);
    UPROPERTY()
    ABaseWitch* PlayerPawnRef;

public:
    virtual void ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation) override {};
    virtual void TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation) override {};
    virtual void OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation) override {};
    virtual void OnDeathMonster(AActor* Monster, const FVector& DeathLocation) override {};
};
