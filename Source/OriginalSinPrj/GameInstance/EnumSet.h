// EnumSet.h

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ELevelType : uint8
{
	IntroLevel = 0,
	TitleLevel,
	MatchLevel,
	LobbyLevel,
	MultiLevel,
	CooperationLevel,
	SingleLevel,
	TrainingLevel,
	FarmingLevel
};

UENUM(BlueprintType)
enum class EAddWidgetType : uint8
{
	OptionWidget = 0,
	BuffSelectWidget,
	CharacterSelectWidget,
	GameSettingWidget,
	MapSelectWidget,
	PlayerStateWidget,
	ResultWidget
};

UENUM(BlueprintType)
enum class ESoundDataType : uint8
{
	Level = 0,
	Monster,
	Character,
	Boss
};

UENUM(BlueprintType)
enum class ELevelSoundType : uint8
{
	TitleSound = 0,
	MatchSound,
	LobbySound,
	MultiSound,
	CooperationSound,
	SingleSound,
	TrainingSound,
	FarmingSound
};

UENUM(BlueprintType)
enum class EMonsterSoundType : uint8
{
	Attack = 0,
	Death
};

UENUM(BlueprintType)
enum class ECharacterSoundType : uint8
{
	Attack = 0,
	Death
};

UENUM(BlueprintType)
enum class EBossSoundType : uint8
{
	Attack = 0,
	Death
};