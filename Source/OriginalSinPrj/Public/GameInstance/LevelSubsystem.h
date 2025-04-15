// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OriginalSinPrj/GameInstance/EnumSet.h"
#include "LevelSubsystem.generated.h"


UCLASS()
class ORIGINALSINPRJ_API ULevelSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	const ELevelType GetCurrentLevel() const;
	void CompareMapName();
	
	void ResponseServerTravelByType(ELevelType Type);
	void ResponseOpenSingleLevelByType(ELevelType Type);
	void ResponseOpenListenServer(ELevelType Type);
	void ResponseClientEnterToServer(ELevelType Type);

	void ConvertTypeToName(ELevelType Type);

private:
	UPROPERTY()
	ELevelType CurrentLevel = ELevelType::IntroLevel;

	UPROPERTY()
	FString TargetName = "";

	static const FName IntroLevelName;
	static const FName TitleLevelName;
	static const FName FarmingLevelName;
	static const FName MatchLevelName;
	static const FName LobbyLevelName;
	static const FName SingleLevelName;
	static const FName MultiLevelName;
	static const FName CooperationLevelName;
	static const FName TrainingLevelName;
};
