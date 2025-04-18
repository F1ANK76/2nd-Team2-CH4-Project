#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "GameFramework/Actor.h"
#include "LevelObjectManager.generated.h"

class ABasePlatform;
class ADeathZone;

UCLASS()
class ORIGINALSINPRJ_API ALevelObjectManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ALevelObjectManager();

	UFUNCTION(BlueprintCallable, Category = "ObjectManager")
	void InitializeTempObjects();

	UFUNCTION(BlueprintCallable, Category = "ObjectManager")
	void SpawnAndDestroyObject();

	UFUNCTION(BlueprintCallable, Category = "ObjectManager")
	void SpawnDeathZone();

	UFUNCTION(BlueprintCallable, Category = "ObjectManager")
	AActor* GetRespawnPlatform();

	FVector GetTopPlatformLocation();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "ObjectManager")
	TArray<AActor*> PooledObjects;

	TQueue<AActor*> TempObjects;

	UPROPERTY(EditAnywhere, Category = "ObjectManager")
	TSubclassOf<ABasePlatform> ObjectToPool;

	UPROPERTY(EditAnywhere, Category = "ObjectManager")
	TSubclassOf<AActor> DeathZoneClass;

	UPROPERTY()
	ADeathZone* DeathZoneInstance;

	UPROPERTY(EditAnywhere, Category = "ObjectManager")
	int32 PoolSize;

	UPROPERTY(EditAnywhere, Category = "ObjectManager")
	int32 PoolIndex;

	int TempObjectSize;
	float ObjectDistance;
	float ObjectZLocation;
	FVector TopPlatformLocation;

private:

};
