// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "PlatformSpawnTarget.generated.h"

UCLASS()
class ORIGINALSINPRJ_API APlatformSpawnTarget : public ATargetPoint
{
	GENERATED_BODY()

public:
	APlatformSpawnTarget();

	bool GetbIsUsed() const { return bIsUsed; }
	void SetUsed() { bIsUsed = true; }
	void SetUnUsed() { bIsUsed = false; }

private:
	UPROPERTY()
	bool bIsUsed;
};
