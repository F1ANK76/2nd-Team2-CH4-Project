// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuffDataArrayStruct.generated.h"

struct FBuffDataStruct;

USTRUCT(BlueprintType)
struct ORIGINALSINPRJ_API FBuffDataArrayStruct
{
	GENERATED_BODY()
	
public:
	TArray<FBuffDataStruct*> BuffDataArray;
};
