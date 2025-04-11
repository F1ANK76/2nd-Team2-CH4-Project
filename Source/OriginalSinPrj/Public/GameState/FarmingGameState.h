// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OriginalSinPrj/Interface/BattleEvent.h"
#include "OriginalSinPrj/Interface/MatchManage.h"
#include "../Widget/LevelWidget/BattleWidget.h"
#include "../Player/BaseWitch.h"
#include "GameFramework/GameState.h"
#include "FarmingGameState.generated.h"


//���߿� �и��� �ؾ�...
USTRUCT(BlueprintType)
struct FPlayerFarmingInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    APlayerState* PlayerState; // ��Ʈ�ѷ� ��� ��� (PlayerState�� ���� ����!)

    UPROPERTY(BlueprintReadWrite)
    int32 Experience;

    UPROPERTY(BlueprintReadWrite)
    int32 Level;
};


UCLASS()
class ORIGINALSINPRJ_API AFarmingGameState : public AGameState, public IBattleEvent, public IMatchManage
{
	GENERATED_BODY()
	
    
public:
    AFarmingGameState();
    
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(Replicated)
    bool bIsFarmingStarted;


    UPROPERTY(Replicated, BlueprintReadOnly)
    float TimeRemaining;

    UPROPERTY(Replicated)
    TArray<FPlayerFarmingInfo> PlayerInfos;


    void AddExperienceToPlayer(APlayerController* Player, int32 Amount);
    
    void StartFarmingMode();

    void EndFarmingMode(); // 0�� ���� �� ó���� �Լ�
public:
    // UI ���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowUI(APlayerController* PlayerController);


    UFUNCTION(BlueprintCallable, Category = "UI")
    UBattleWidget* GetBattleWidgetFromController(APlayerController* PlayerController);
private:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UBattleWidget> BattleWidgetClass; // UI Ŭ����

protected:
    virtual void BeginPlay() override;

    

    FTimerHandle TimerHandle;
    void CheckLevelUp(APlayerController* Player);

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    
public:
    APlayerController* GetPlayerController() const;

    void SetPlayerPawn(ABaseWitch* InPawn);
    ABaseWitch* GetPlayerPawn() const;

private:
    UPROPERTY()
    ABaseWitch* PlayerPawnRef;







public:

    virtual void StartMatch() override {};

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
