// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../EnumSet.h"
#include "LevelAudioDataStruct.generated.h"

USTRUCT(BlueprintType)
struct ORIGINALSINPRJ_API FLevelAudioDataStruct : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ELevelSoundType LevelSoundType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<USoundBase> Sound;
};
