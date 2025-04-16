#pragma once

#include "CoreMinimal.h"
#include "OriginalSinPrj/Interface/BattleEvent.h"
#include "OriginalSinPrj/Interface/MatchManage.h"
#include "Player/Struct/CharacterStateBuffer.h"
#include "GameFramework/GameState.h"
#include "OriginalSinPrj/Widget/AddedWidget/PlayerStateWidget.h"
#include "MultiBattleGameState.generated.h"

class AMultiBattleGameMode;

UCLASS()
class ORIGINALSINPRJ_API AMultiBattleGameState : public AGameState, public IBattleEvent, public IMatchManage
{
	GENERATED_BODY()
	
public:
	AMultiBattleGameState();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation) override;
	virtual void TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation) override;
	virtual void OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation) override;
	virtual void OnDeathMonster(AActor* Monster, const FVector& DeathLocation) override;
	virtual void FinishMatch() override;
	virtual void VictoryMatch() override;
	virtual void DefeatMatch() override;
	virtual void DrawMatch() override;

	void InitPlayerInfo();
	void UpdatePlayerInfo(const FCharacterStateBuffer& State);
	void SetStage2CameraTransform();
	void InitPlayerUIInfo();
	void UpdatePlayerUIInfo();
	void RegisterInitialController(APlayerController* PC);

	UFUNCTION()
	void OnRep_UpdatePlayerInitData();

	UFUNCTION()
	void OnRep_UpdatePlayer1DataUI();

	UFUNCTION()
	void OnRep_UpdatePlayer2DataUI();

	UPROPERTY(ReplicatedUsing = OnRep_UpdatePlayer1DataUI)
	int Player1DataChanged = 0;

	UPROPERTY(ReplicatedUsing = OnRep_UpdatePlayer2DataUI)
	int Player2DataChanged = 0;

private:

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAttackPlayerAnimation(AActor* Attacker);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayBeAttackedPlayerAnimation(AActor* Attacker);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAttackMonsterAnimation(AActor* Attacker);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayBeAttackedMonsterAnimation(AActor* Attacker);

protected:
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Camera")
	FVector CameraLocation;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Camera")
	FRotator CameraRotation;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Camera")
	float CameraDistance;
	
public:

	UPROPERTY()
	TArray<TWeakObjectPtr<APlayerController>> PlayerControllerSet;
	
	UPROPERTY(Replicated)
	FPlayerData Player1StateData;

	UPROPERTY(Replicated)
	FPlayerData Player2StateData;
	
	AMultiBattleGameMode* MultiBattleGameMode = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<AActor*, FPlayerData> PlayerInfos;
};