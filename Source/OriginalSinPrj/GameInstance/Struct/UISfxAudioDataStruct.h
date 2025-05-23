// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../EnumSet.h"
#include "UISfxAudioDataStruct.generated.h"

USTRUCT(BlueprintType)
struct ORIGINALSINPRJ_API FUISfxAudioDataStruct : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EUISfxSoundType UISfxSoundType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<USoundBase> Sound;
};
