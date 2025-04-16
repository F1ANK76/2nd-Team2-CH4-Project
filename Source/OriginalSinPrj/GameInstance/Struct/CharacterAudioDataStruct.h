// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../EnumSet.h"
#include "CharacterAudioDataStruct.generated.h"


USTRUCT(BlueprintType)
struct ORIGINALSINPRJ_API FCharacterAudioDataStruct : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECharacterSoundType CharacterSoundType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<USoundBase> Sound;
};
