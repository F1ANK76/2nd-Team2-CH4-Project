// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OriginalSinPrj/GameInstance/EnumSet.h"
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
};
