#pragma once

#include "CoreMinimal.h"
#include "OriginalSinPrj/Interface/BattleEvent.h"
#include "OriginalSinPrj/Interface/MatchManage.h"
#include "GameFramework/GameMode.h"
#include "MultiBattleGameMode.generated.h"

class ALevelObjectManager;

UCLASS()
class ORIGINALSINPRJ_API AMultiBattleGameMode : public AGameMode, public IBattleEvent, public IMatchManage
{
	GENERATED_BODY()
	
public:
	AMultiBattleGameMode();

	virtual void BeginPlay() override;

	void StartDelay();
	void StartToSpawnActor();

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

	/*테스트용*/
	/*UFUNCTION(BlueprintCallable)
	void CreateTestPlatform(FVector SpawnLocation, FRotator SpawnRotator);*/

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reveal Actor")
	TArray<AActor*> ActorArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reveal Actor")
	TSubclassOf<ALevelObjectManager> LevelObjectManagerClass;
private:

	UPROPERTY()
	ALevelObjectManager* LevelObjectManager;

	FTimerHandle ActorRevealTimer;
	int32 CurrentActorArrayIndex;

};
