// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Player/WitchTypes.h"
#include "OriginalSinPrj/GameInstance/Struct/CharacterTypeData.h"
#include "OriginalSinPrj/GameInstance/Struct/BuffDataArrayStruct.h"
#include "DataSubsystem.generated.h"

class UAllDataSettings;
struct FCharacterDataStruct;
struct FBuffDataStruct;

UCLASS()
class ORIGINALSINPRJ_API UDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	const FCharacterDataStruct* GetCharacterDataByType(ECharacterType Type);
	const TArray<FBuffDataStruct*>& GetBuffDataArray();
	const int32 GetCharacterTypeSize();

	//void LoadDataTables();


private:
	bool CheckValidOfCharacterData();
	bool CheckValidOfBuffData();

private:
	UPROPERTY()
	FCharacterTypeData CharacterTypeData;

	UPROPERTY()
	FBuffDataArrayStruct BuffDataArrayStruct;

	UPROPERTY()
	UDataTable* BuffDataTable;

	UPROPERTY()
	UDataTable* CharacterDataTable;

	UPROPERTY()
	const UAllDataSettings* AllDataSettings;
	/*UPROPERTY()
	TArray<FCharacterDataStruct*> CharacterDataArray;*/
};
