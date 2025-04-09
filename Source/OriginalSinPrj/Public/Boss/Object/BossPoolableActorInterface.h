// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BossPoolableActorInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBossPoolableActorInterface : public UInterface
{
	GENERATED_BODY()
};

class ORIGINALSINPRJ_API IBossPoolableActorInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnPooledObjectSpawn();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnPooledObjectReset();
};
