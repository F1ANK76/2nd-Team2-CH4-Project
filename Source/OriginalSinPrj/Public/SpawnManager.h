#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

class AWitchController;

UCLASS()
class ORIGINALSINPRJ_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SpawnPlayer(AWitchController* Controller, FVector SpawnLocation);
	void SpawnMonster(AController* Controller, FVector SpawnLocation);

	UPROPERTY(EditAnywhere, Category = "SpawnPlayer")
	TSubclassOf<APawn> PlayerClass;

	UPROPERTY(EditAnywhere, Category = "SpawnMonster")
	TSubclassOf<APawn> MonsterClass;
};
