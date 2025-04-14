// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "../Widget/LevelWidget/CooperationWidget.h"
#include "../Widget/AddedWidget/PlayerStateWidget.h"
#include "../Player/BaseWitch.h"
#include "OriginalSinPrj/Interface/CameraStateInterface.h"
#include "CooperationGameState.generated.h"

struct FBuffType;
class ACooperationGameMode;


UCLASS()
class ORIGINALSINPRJ_API ACooperationGameState : public AGameState, public ICameraStateInterface
{
	GENERATED_BODY()


public:
    virtual FVector GetCameraLocation() const override { return CameraLocation; }
    virtual FRotator GetCameraRotation() const override { return CameraRotation; }
    virtual float GetCameraDistance() const override { return CameraDistance; }



public:
    ACooperationGameState();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;


    ACooperationGameMode* CooperationGameGameMode;

    UPROPERTY(Replicated)
    bool bPlayer1SelectedBuff = false;

    UPROPERTY(Replicated)
    bool bPlayer2SelectedBuff = false;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

  

    UPROPERTY(Replicated)
    bool bIsStage3Started;

    UPROPERTY(Replicated, BlueprintReadOnly)
    float Timer;

    void TurnOnTimer(); //������ Ÿ�̸� �ѱ�

    // Player ���� ����
    UPROPERTY(BlueprintReadOnly)
    TMap<APlayerController*, FPlayerData> PlayerInfos;

    //�÷��̾� ��Ʈ�ѷ� �����س���
    UPROPERTY()
    TArray<TWeakObjectPtr<APlayerController>> PlayerControllerSet;
    void RegisterInitialController(APlayerController* PC);

    void InitPlayerInfo();
    void UpdatePlayerInfo();
    void InitPlayerUIInfo();
    void UpdatePlayerUIInfo();

    void RequestPlayerToOpenBuffUI();//�÷��̾�� ���� ���� UI ������ ��Ű��

    void RequestPlayerToOpenResultUI(); //�÷��̾�� ��� UI ������ ��Ű��

    void ReceiveSelectedBuff(APlayerController* player, FBuffType* Bufftype); // �÷��̾� UI���� ���õ� ���� ���� �ް� ���ٰ� ������ѳ���.
    
    FBuffType* Player1Stage1SelectedBuff;
    FBuffType* Player2Stage1SelectedBuff;

    FBuffType* Player1Stage2SelectedBuff;
    FBuffType* Player2Stage2SelectedBuff;
    

    void ApplyBuffStat(); // ���Ӹ�尡 ������� ���� �����Ű��

    void AddExperienceToPlayer(APlayerController* Player, int32 Amount);

    UPROPERTY(ReplicatedUsing = OnRep_CameraLocation, BlueprintReadWrite, Category = "Camera")
    FVector CameraLocation;

    UPROPERTY(ReplicatedUsing = OnRep_CameraLocation, BlueprintReadWrite, Category = "Camera")
    FRotator CameraRotation;

    UPROPERTY(ReplicatedUsing = OnRep_CameraLocation, BlueprintReadWrite, Category = "Camera")
    float CameraDistance;


    //ī�޶� ��ġ �����ϱ�... //
    void SetStage1CameraTransform();

    void SetStage2CameraTransform();

    void SetStage3CameraTransform();

    //������

    UFUNCTION()
    void OnRep_CameraLocation()
    {
        // ī�޶� ��ġ ���� �� ȣ��Ǵ� �ڵ�
        UE_LOG(LogTemp, Warning, TEXT("CameraLocation changed: %s"), *CameraLocation.ToString());
    }



    UFUNCTION()
    void OnRep_CameraRotation()
    {
        // ī�޶� ȸ�� ���� �� ȣ��Ǵ� �ڵ�
        UE_LOG(LogTemp, Warning, TEXT("CameraRotation changed: %s"), *CameraRotation.ToString());
    }


    UFUNCTION()
    void OnRep_CameraDistance()
    {
        // ī�޶� �Ÿ� ���� �� ȣ��Ǵ� �ڵ�
        UE_LOG(LogTemp, Warning, TEXT("CameraDistance Changed: %f"), CameraDistance);
    }



private:

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UCooperationWidget> CooperationWidgetClass; // UI Ŭ����


    FTimerHandle TimerHandle;
    void CheckLevelUp(APlayerController* Player);

public:
    void SetPlayerPawn(ABaseWitch* InPawn);
private:
    UPROPERTY()
    ABaseWitch* PlayerPawnRef;
	
};
