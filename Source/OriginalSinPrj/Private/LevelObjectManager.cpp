#include "LevelObjectManager.h"
#include "LevelObject/BasePlatform.h"
#include "DeathZone.h"

ALevelObjectManager::ALevelObjectManager()
	: PoolSize(12)
	, ObjectDistance(100.f)
	, ObjectZLocation(0.f)
	, TempObjectSize(4)
	, PoolIndex(4)
{
 	PrimaryActorTick.bCanEverTick = false;
	ObjectToPool = ABasePlatform::StaticClass();
}

void ALevelObjectManager::BeginPlay()
{
	Super::BeginPlay();
	
	if (UWorld* World = GetWorld())
	{
		for (int i = 0; i < PoolSize; i++)
		{
			FVector NewActorLocation(400.0f, 0.0f, ObjectZLocation);
			FVector NewActorScale(1.0f, 1.0f, 1.0f);
			FRotator NewActorRotator(0.0f, 0.0f, 0.0f);
			FTransform NewActorTransform(NewActorRotator, NewActorLocation, NewActorScale);

			AActor* NewActor = World->SpawnActor<ABasePlatform>(ObjectToPool, NewActorTransform);
			if (NewActor)
			{
				NewActor->SetActorHiddenInGame(true);
				NewActor->SetActorEnableCollision(false);
				NewActor->SetActorTickEnabled(false);
				PooledObjects.Add(NewActor);
			}

			ObjectZLocation += ObjectDistance;
		}
	}
}

void ALevelObjectManager::InitializeTempObjects()
{
	for (int i = 0; i < TempObjectSize; i++)
	{
		if (PooledObjects.IsValidIndex(i))
		{
			AActor* TempActor = PooledObjects[i];
			TempActor->SetActorHiddenInGame(false);
			TempActor->SetActorEnableCollision(true);
			TempActor->SetActorTickEnabled(true);

			TempObjects.Enqueue(TempActor);
		}
	}
}

void ALevelObjectManager::SpawnAndDestroyObject()
{
	AActor* DeletedObject;
	if (!TempObjects.IsEmpty())
	{
		if (TempObjects.Dequeue(DeletedObject))
		{
			DeletedObject->SetActorHiddenInGame(true);
			DeletedObject->SetActorEnableCollision(false);
			DeletedObject->SetActorTickEnabled(false);

			UE_LOG(LogTemp, Warning, TEXT("액터 비활성화"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TempObjects가 비었음"));
	}

	if (PooledObjects.IsValidIndex(PoolIndex))
	{
		AActor* TempActor = PooledObjects[PoolIndex];

		TempActor->SetActorHiddenInGame(false);
		TempActor->SetActorEnableCollision(true);
		TempActor->SetActorTickEnabled(true);

		TempObjects.Enqueue(TempActor);
		PoolIndex++;
		UE_LOG(LogTemp, Warning, TEXT("액터 활성화 : %d"), PoolIndex);
	}
}

void ALevelObjectManager::SpawnDeathZone()
{
	FVector NewActorLocation(0.0f, 0.0f, -150.0f);
	FVector NewActorScale(1.0f, 1.0f, 1.0f);
	FRotator NewActorRotator(0.0f, 0.0f, 0.0f);
	FTransform NewActorTransform(NewActorRotator, NewActorLocation, NewActorScale);

	DeathZoneInstance = GetWorld()->SpawnActor<ADeathZone>(DeathZoneClass, NewActorTransform);
	if (DeathZoneInstance)
	{
		DeathZoneInstance->SetActorHiddenInGame(false);
		DeathZoneInstance->SetActorEnableCollision(true);
		DeathZoneInstance->SetActorTickEnabled(true);

		DeathZoneInstance->OnMyOverlapEvent.AddUObject(this, &ALevelObjectManager::SpawnAndDestroyObject);
	}
}
