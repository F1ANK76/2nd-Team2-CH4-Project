// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OriginalSinPrj/GameInstance/EnumSet.h"
#include "Player/WitchTypes.h"
#include "OriginalSinPrj/GameInstance/Struct/GameSettingData.h"
#include "OriginalSinPrjGameInstance.generated.h"

class UUISubsystem;
class UAudioSubsystem;
class UDataSubsystem;
class ULevelSubsystem;
class APlayerController;

UCLASS()
class ORIGINALSINPRJ_API UOriginalSinPrjGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	void SetOwningPlayerController(APlayerController* OwningPC);
	APlayerController* GetOwningPlayerController() const;

	void RequestOpenLevel(const FString& LevelName);
	void RequestOpenLevelByType(ELevelType Type, bool bIsSingle);
	void ResponseShowWidget();

	FGameSettingData& GetGameSettingData();

	ECharacterType GetSelectedCharacterType() const;
	int32 GetPlayerFarmingLevel() const;

	void SetSelectedCharacterType(ECharacterType SelectedType);
	void SetPlayerFarmingLevel(int32 LevelValue);

	void SetSelectedMapIndex(int32 Index);

	const ELevelType GetCurrentLevelType();

private:
	UPROPERTY()
	UUISubsystem* UISubsystem;

	UPROPERTY()
	UAudioSubsystem* AudioSubsystem;

	UPROPERTY()
	UDataSubsystem* DataSubsystem;

	UPROPERTY()
	TObjectPtr<ULevelSubsystem> LevelSubsystem = nullptr;

	UPROPERTY()
	TObjectPtr<APlayerController> OwningPlayer = nullptr;

	UPROPERTY()
	FGameSettingData GameSetting;

	UPROPERTY()
	ECharacterType CharacterType = ECharacterType::Hand;

	UPROPERTY()
	int32 FarmingLevel = 0;
};
