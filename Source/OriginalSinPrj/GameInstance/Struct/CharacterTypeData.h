// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypeData.generated.h"

struct FCharacterDataStruct;

USTRUCT(BlueprintType)
struct ORIGINALSINPRJ_API FCharacterTypeData
{
	GENERATED_BODY()

public:
	TArray<FCharacterDataStruct*> CharacterDataArray;
	
};
