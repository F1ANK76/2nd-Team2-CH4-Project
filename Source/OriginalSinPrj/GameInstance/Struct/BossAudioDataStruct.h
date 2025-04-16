// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../EnumSet.h"
#include "BossAudioDataStruct.generated.h"

USTRUCT(BlueprintType)
struct ORIGINALSINPRJ_API FBossAudioDataStruct : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EBossSoundType BossSoundType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<USoundBase> Sound;
};
