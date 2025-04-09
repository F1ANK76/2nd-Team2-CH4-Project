// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossPoolableActorInterface.h"
#include "GameFramework/Actor.h"
#include "DestructibleObject.generated.h"

UCLASS()
class ORIGINALSINPRJ_API ADestructibleObject : public AActor, public IBossPoolableActorInterface
{
	GENERATED_BODY()

public:
	ADestructibleObject();

protected:
	virtual void BeginPlay() override;
	virtual void OnPooledObjectSpawn_Implementation() override;
	virtual void OnPooledObjectReset_Implementation() override;
};