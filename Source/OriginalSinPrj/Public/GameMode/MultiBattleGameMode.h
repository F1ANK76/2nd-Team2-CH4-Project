#pragma once

#include "CoreMinimal.h"
#include "GameState/MultiBattleGameState.h"
#include "OriginalSinPrj/Interface/BattleEvent.h"
#include "Player/Struct/CharacterStateBuffer.h"
#include "OriginalSinPrj/Interface/MatchManage.h"
#include "GameFramework/GameMode.h"
#include "BaseCamera.h"
#include "MultiBattleGameMode.generated.h"

class ALevelObjectManager;
class ASpawnManager;
class ABaseWitch;

UCLASS()
class ORIGINALSINPRJ_API AMultiBattleGameMode : public AGameMode, public IBattleEvent, public IMatchManage
{
	GENERATED_BODY()
	
public:
	AMultiBattleGameMode();

	virtual void BeginPlay() override;

	void StartDelay();
	void StartGame();
	void SpawnPlayer();
	void RespawnPlayer(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	virtual void ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnDeathMonster(AActor* Monster, const FVector& DeathLocation) override;
		
	UFUNCTION(BlueprintCallable)
	virtual void FinishMatch() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void VictoryMatch() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void DefeatMatch() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void DrawMatch() override;

	void InitializeTempObjects();
	void SpawnAndDestroyObject();
	void SpawnCamera();
	void AttachPlayerToCamera(ACharacter* Player, ABaseCamera* Camera);
	void InitPlayerUI();

	UFUNCTION()
	void OnCharacterStateReceived(const FCharacterStateBuffer& State);
	
	void HandleClientPossession(APlayerController* PC, int index);

	void RequestUpdateUI(int PlayerIndex)
	{
		MultiBattleGameState->UpdatePlayerUIInfo();
		if (PlayerIndex == 0)
		{
			MultiBattleGameState->Player1DataChanged++;
		}
        
		if (PlayerIndex == 1)
		{
			MultiBattleGameState->Player2DataChanged++;
		}
	}

	virtual void PostSeamlessTravel() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectManager")
	TSubclassOf<ALevelObjectManager> LevelObjectManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnManager")
	TSubclassOf<ASpawnManager> SpawnManagerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<ABaseCamera> BaseCamera;

	TObjectPtr<AMultiBattleGameState> MultiBattleGameState = nullptr;
	TArray<AActor*> ActivePlayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
	TArray<AActor*> ActorArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BossCamera")
	TArray<FVector> CameraSpawnLocations;

	UPROPERTY(BlueprintReadWrite)
	TArray<ABaseWitch*> SpawnedCharacters;

	UPROPERTY()
	TArray<ABaseCamera*> SpawnedBaseCamera;
	
private:

	UPROPERTY()
	ALevelObjectManager* LevelObjectManager;

	UPROPERTY()
	ASpawnManager* SpawnManager;

	FTimerHandle ActorRevealTimer;
	int32 CurrentActorArrayIndex;

};
