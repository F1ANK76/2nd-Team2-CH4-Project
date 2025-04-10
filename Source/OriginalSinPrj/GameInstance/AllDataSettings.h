// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AllDataSettings.generated.h"

/**
 * 
 */
UCLASS(Config = AllDataSettings, DefaultConfig, meta = (DisplayName = "AllData Settings"))
class ORIGINALSINPRJ_API UAllDataSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Config, Category = "Buff")
    TSoftObjectPtr<UDataTable> BuffDataTable;
};
