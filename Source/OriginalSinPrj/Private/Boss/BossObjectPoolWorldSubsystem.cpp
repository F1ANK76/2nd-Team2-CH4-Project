// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossObjectPoolWorldSubsystem.h"
#include "Boss/Object/BossPoolableActorInterface.h"
#include "Boss/Object/BossPoolObjectDataAsset.h"
#include "Boss/Object/RangeAttackProjectile.h"
#include "GameFramework/GameMode.h"

void UBossObjectPoolWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	PoolConfig = LoadObject<UBossPoolObjectDataAsset>(nullptr, TEXT("/Game/Resources/Boss/DA_BossPoolObject"));
	
	if (!IsValid(PoolConfig))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Load BossPoolObject DataAsset"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("BossObjectPoolWorldSubsystem Initialized in Level : %s"), *GetWorld()->GetName());
	}
}

void UBossObjectPoolWorldSubsystem::Deinitialize()
{
	Super::Deinitialize();
	ObjectPools.Empty();
}

bool UBossObjectPoolWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* World = Cast<UWorld>(Outer);
	if (IsValid(World))
	{
		FString CurrentLevelName = World->GetName();

		return (
			//추후 죄종본에서는 수정 필요함
			CurrentLevelName.Contains("Boss") ||
			CurrentLevelName.Contains("Coop")
		);
	}
	return false;
}

template <typename T>
T* UBossObjectPoolWorldSubsystem::SpawnPooledActor(TSubclassOf<AActor> ClassToSpawn, const FVector& Location, const FRotator& Rotation)
{
	if (!IsValid(ClassToSpawn)) return nullptr;

	FActorPoolList* PoolList = ObjectPools.Find(ClassToSpawn);
	if (!PoolList)
	{
		FActorPoolList NewList;
		ObjectPools.Add(ClassToSpawn, NewList);
		PoolList = ObjectPools.Find(ClassToSpawn);
	}
	//1. 재사용
	for (TObjectPtr<AActor>& PooledActor : PoolList->PooledActors)
	{
		if (PooledActor->IsHidden())
		{
			PooledActor->SetActorLocationAndRotation(Location, Rotation);

			if (PooledActor->Implements<UBossPoolableActorInterface>())
			{
				IBossPoolableActorInterface::Execute_OnPooledObjectSpawn(PooledActor);
			}

			return Cast<T>(PooledActor);
		}
	}

	//2. 새로 생성
	UWorld* World = GetWorld();
	if (!IsValid(World)) return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.bNoFail = true;
	SpawnParams.Owner = GetWorld()->GetAuthGameMode();
	
	AActor* NewActor = World->SpawnActor<AActor>(ClassToSpawn, Location, Rotation, SpawnParams);
	if (IsValid(NewActor))
	{
		NewActor->SetReplicates(true);
		NewActor->SetReplicateMovement(true);
		PoolList->PooledActors.Add(NewActor);
		AllPooledActors.Add(NewActor);

		if (NewActor->Implements<UBossPoolableActorInterface>())
		{
			IBossPoolableActorInterface::Execute_OnPooledObjectSpawn(NewActor);
		}
	}
	return Cast<T>(NewActor);
}

void UBossObjectPoolWorldSubsystem::ReturnActorToPool(AActor* Actor)
{
	if (!IsValid(Actor)) return;
	if (Actor->Implements<UBossPoolableActorInterface>())
	{
		IBossPoolableActorInterface::Execute_OnPooledObjectReset(Actor);
	}
}

ARangeAttackProjectile* UBossObjectPoolWorldSubsystem::SpawnRangeAttackProjectile(const FVector& Location, const FRotator& Rotation)
{
	if (IsValid(PoolConfig->RangeAttackProjectileClass))
	{
		return SpawnPooledActor<ARangeAttackProjectile>(PoolConfig->RangeAttackProjectileClass, Location, Rotation);
	}

	UE_LOG(LogTemp, Error, TEXT("No RangeAttackProjectileClass in Data Asset"));
	return nullptr;
}

AWeaponToSpawn* UBossObjectPoolWorldSubsystem::SpawnWeaponToSpawn(const FVector& Location, const FRotator& Rotation)
{
	if (IsValid(PoolConfig->WeaponToSpawnClass))
	{
		return SpawnPooledActor<AWeaponToSpawn>(PoolConfig->WeaponToSpawnClass, Location, Rotation);
	}

	UE_LOG(LogTemp, Error, TEXT("No WeaponToSpawnClass in Data Asset"));
	return nullptr;
}

ARushBossClone* UBossObjectPoolWorldSubsystem::SpawnRushBossClone(const FVector& Location, const FRotator& Rotation)
{
	if (IsValid(PoolConfig->RushBossCloneClass))
	{
		return SpawnPooledActor<ARushBossClone>(PoolConfig->RushBossCloneClass, Location, Rotation);
	}

	UE_LOG(LogTemp, Error, TEXT("No RushBossCloneClass in Data Asset"));
	return nullptr;
}

ADestructibleObject* UBossObjectPoolWorldSubsystem::SpawnDestructibleObject(const FVector& Location,
	const FRotator& Rotation)
{
	if (IsValid(PoolConfig->DestructibleObjectClass))
	{
		return SpawnPooledActor<ADestructibleObject>(PoolConfig->DestructibleObjectClass, Location, Rotation);
	}

	UE_LOG(LogTemp, Error, TEXT("No DestructibleObjectClass in Data Asset"));
	return nullptr;
}
