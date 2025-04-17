// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DestructibleObject.h"
#include "RangeAttackProjectile.h"
#include "WeaponToSpawn.h"
#include "RushBossClone.h"
#include "BossPlatform.h"
#include "IndexPatternProjectile.h"
#include "Engine/DataAsset.h"
#include "BossPoolObjectDataAsset.generated.h"

class AIndexPatternProjectile;

UCLASS(BlueprintType)
class ORIGINALSINPRJ_API UBossPoolObjectDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "BossPoolObject")
	TSubclassOf<ARangeAttackProjectile> RangeAttackProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "BossPoolObject")
	TSubclassOf<AWeaponToSpawn> WeaponToSpawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "BossPoolObject")
	TSubclassOf<ARushBossClone> RushBossCloneClass;

	UPROPERTY(EditDefaultsOnly, Category = "BossPoolObject")
	TSubclassOf<ADestructibleObject> DestructibleObjectClass;

	UPROPERTY(EditDefaultsOnly, Category = "BossPoolObject")
	TSubclassOf<ABossPlatform> BossPlatformClass;

	UPROPERTY(EditDefaultsOnly, Category = "BossPoolObject")
	TSubclassOf<AIndexPatternProjectile> IndexPatternProjectileClass;
};
