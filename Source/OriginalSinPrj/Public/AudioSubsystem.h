// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ORIGINALSINPRJ_API UAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void PlayBGM();
};
