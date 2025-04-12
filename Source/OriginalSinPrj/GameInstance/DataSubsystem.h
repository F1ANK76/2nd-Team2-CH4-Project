// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataSubsystem.generated.h"

class UAllDataSettings;

UCLASS()
class ORIGINALSINPRJ_API UDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void LoadDataTables();

	UPROPERTY()
	UDataTable* BuffDataTable;

	UPROPERTY()
	UDataTable* CharacterDataTable;

	const UAllDataSettings* AllDataSettings;
};
