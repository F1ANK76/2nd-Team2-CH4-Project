// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/LevelSubsystem.h"
#include "Kismet/GameplayStatics.h"

const FName ULevelSubsystem::IntroLevelName = "IntroLevel";
const FName ULevelSubsystem::TitleLevelName = "TitleLevel";
const FName ULevelSubsystem::TrainingLevelName = "TrainingLevel";
const FName ULevelSubsystem::FarmingLevelName = "FarmingLevel";
const FName ULevelSubsystem::SingleLevelName = "SingleLevel";
const FName ULevelSubsystem::MatchLevelName = "MatchLevel";
const FName ULevelSubsystem::LobbyLevelName = "LobbyLevel";
const FName ULevelSubsystem::MultiLobbyLevelName = "MultiLobbyLevel";
const FName ULevelSubsystem::MultiLevelName = "MultiLevel";	// 테스트 용으로 이름 잠시 변경
const FName ULevelSubsystem::CooperationLevelName = "CooperationLevel";

const ELevelType ULevelSubsystem::GetCurrentLevel() const
{
	return CurrentLevel;
}

void ULevelSubsystem::ResponseServerTravelByType(ELevelType Type)
{
	CurrentLevel = Type;

	ConvertTypeToName(Type);

	if (IsValid(GetWorld()))
	{
		GetWorld()->ServerTravel(TargetName);
	}
}

void ULevelSubsystem::ResponseOpenSingleLevelByType(ELevelType Type)
{
	CurrentLevel = Type;

	ConvertTypeToName(Type);

	if (IsValid(GetWorld()))
	{
		UGameplayStatics::OpenLevel(GetWorld(), (FName)TargetName);
	}
}

void ULevelSubsystem::ResponseOpenListenServer(ELevelType Type)
{
	CurrentLevel = Type;

	ConvertTypeToName(Type);

	if (IsValid(GetWorld()))
	{

	}
}

void ULevelSubsystem::ResponseClientEnterToServer(ELevelType Type)
{
	CurrentLevel = Type;

	ConvertTypeToName(Type);

	if (IsValid(GetWorld()))
	{

	}
}

void ULevelSubsystem::ConvertTypeToName(ELevelType Type)
{
	switch (CurrentLevel)
	{
	case ELevelType::IntroLevel:
		TargetName = IntroLevelName.ToString();
		break;

	case ELevelType::TitleLevel:
		TargetName = TitleLevelName.ToString();
		break;

	case ELevelType::TrainingLevel:
		TargetName = TrainingLevelName.ToString();
		break;

	case ELevelType::FarmingLevel:
		TargetName = FarmingLevelName.ToString();
		break;

	case ELevelType::SingleLevel:
		TargetName = SingleLevelName.ToString();
		break;

	case ELevelType::MatchLevel:
		TargetName = MatchLevelName.ToString();
		break;

	case ELevelType::LobbyLevel:
		TargetName = LobbyLevelName.ToString();
		break;
		
	case ELevelType::MultiLobbyLevel:
		TargetName = MultiLobbyLevelName.ToString();
		break;

	case ELevelType::MultiLevel:
		TargetName = MultiLevelName.ToString();
		break;

	case ELevelType::CooperationLevel:
		TargetName = CooperationLevelName.ToString();
		break;
	}
}

void ULevelSubsystem::CompareMapName()
{
	FString MapName = GetWorld()->GetMapName();

	if (MapName.Contains(IntroLevelName.ToString()))
	{
		CurrentLevel = ELevelType::IntroLevel;
	}
	else if (MapName.Contains(TitleLevelName.ToString()))
	{
		CurrentLevel = ELevelType::TitleLevel;
	}
	else if (MapName.Contains(TrainingLevelName.ToString()))
	{
		CurrentLevel = ELevelType::TrainingLevel;
	}
	else if (MapName.Contains(FarmingLevelName.ToString()))
	{
		CurrentLevel = ELevelType::FarmingLevel;
	}
	else if (MapName.Contains(SingleLevelName.ToString()))
	{
		CurrentLevel = ELevelType::SingleLevel;
	}
	else if (MapName.Contains(MatchLevelName.ToString()))
	{
		CurrentLevel = ELevelType::MatchLevel;
	}
	else if (MapName.Contains(MultiLobbyLevelName.ToString()))
	{
		CurrentLevel = ELevelType::MultiLobbyLevel;
	}
	else if (MapName.Contains(LobbyLevelName.ToString()))
	{
		CurrentLevel = ELevelType::LobbyLevel;
	}
	else if (MapName.Contains(MultiLevelName.ToString()))
	{
		CurrentLevel = ELevelType::MultiLevel;
	}
	else if (MapName.Contains(CooperationLevelName.ToString()))
	{
		CurrentLevel = ELevelType::CooperationLevel;
	}
}