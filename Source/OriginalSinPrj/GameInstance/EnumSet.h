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
enum class ESfxSoundType : uint8
{
	UI,
	Monster,
	Character,
	Boss
};

UENUM(BlueprintType)
enum class ELevelSoundType : uint8
{
	IntroSound = 0,
	TitleSound,
	MatchSound,
	LobbySound,
	MultiSound,
	CooperationSound,
	SingleSound,
	TrainingSound,
	FarmingSound
};

UENUM(BlueprintType)
enum class EUISfxSoundType : uint8
{
	Click = 0,
	OK
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

UENUM(BlueprintType)
enum class EBuffType : uint8
{
	ManaUp = 0,
	AttackUp,
	DefenseUp,
	SpeedUp,
	AvoidanceUp,
	RespawnInvincible,
	CircleUp,
	LifePointUp,
	DoubleJump,
	AttackSpeedUp,
	AttackDelayDown,
	Stealth,
	Parachute,
	EnemyCircleDown,
	ProjectileSizeUp,
	AttackRangeUp
};