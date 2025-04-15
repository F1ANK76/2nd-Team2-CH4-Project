// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OriginalSinPrjGameInstance.generated.h"

class UUISubsystem;
class UAudioSubsystem;
class UDataSubsystem;
class UMyOnlineSubsystem;

UCLASS()
class ORIGINALSINPRJ_API UOriginalSinPrjGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

private:
	UPROPERTY()
	UUISubsystem* UISubsystem;

	UPROPERTY()
	UAudioSubsystem* AudioSubsystem;

	UPROPERTY()
	UDataSubsystem* DataSubsystem;

	UPROPERTY()
	UMyOnlineSubsystem* MyOnlineSubsystem;
};
