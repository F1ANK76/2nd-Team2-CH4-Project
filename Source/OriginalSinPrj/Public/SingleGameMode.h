#pragma once

#include "CoreMinimal.h"
#include "OriginalSinPrj/Interface/BattleEvent.h"
#include "GameFramework/GameMode.h"
#include "SingleGameMode.generated.h"

class ALevelObjectManager;
class ASpawnManager;

UCLASS()
class ORIGINALSINPRJ_API ASingleGameMode : public AGameMode, public IBattleEvent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void StartToSpawnActor();
	void SpawnPlayer();

	UFUNCTION(BlueprintCallable)
	virtual void ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnDeathMonster(AActor* Monster, const FVector& DeathLocation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectManager")
	TSubclassOf<ALevelObjectManager> LevelObjectManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnManager")
	TSubclassOf<ASpawnManager> SpawnManagerClass;

	UPROPERTY()
	ALevelObjectManager* LevelObjectManager;

	UPROPERTY()
	ASpawnManager* SpawnManager;
};
