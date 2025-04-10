// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnumSet.h"
#include "AudioSubsystem.generated.h"

class UAudioDataSettings;

UCLASS()
class ORIGINALSINPRJ_API UAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void LoadDataTables();
	void PlaySounds(ESoundDataType SoundType, uint8 DetailSoundType);

	UPROPERTY()
	UDataTable* LevelSoundTable;

	UPROPERTY()
	TObjectPtr<UAudioComponent> BgmComp;

	//UWorld* World;

	const UAudioDataSettings* AudioDataSettings;
};
