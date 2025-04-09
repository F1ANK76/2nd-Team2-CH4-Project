#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpawnEvent.generated.h"

UINTERFACE(MinimalAPI)
class USpawnEvent : public UInterface
{
	GENERATED_BODY()
};

class ORIGINALSINPRJ_API ISpawnEvent
{
	GENERATED_BODY()

public:

	UFUNCTION(Category = "Spawn Event")
	virtual void SpawnPlayer(const FTransform& SpawnTransform, TSubclassOf<AActor> PlayerClass, APlayerController* OwningController) = 0;

	UFUNCTION(Category = "Spawn Event")
	virtual void SpawnMonster(const FTransform& SpawnTransform, TSubclassOf<AActor> PlayerClass, int32 MonsterID) = 0;

	UFUNCTION(Category = "Spawn Event")
	virtual void SpawnPlayerAI(const FTransform& SpawnTransform, TSubclassOf<AActor> PlayerAIClass, AController* AIController) = 0;

	UFUNCTION(Category = "Spawn Event")
	virtual void SpawnDummy(const FTransform& SpawnTransform, TSubclassOf<AActor> DummyClass) = 0;
};
