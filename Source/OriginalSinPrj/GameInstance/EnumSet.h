// EnumSet.h

#pragma once

#include "CoreMinimal.h"


class ORIGINALSINPRJ_API EnumSet
{

};

UENUM(BlueprintType)
enum class ELevelType : uint8
{
	TitleLevel = 0,
	MatchLevel,
	LobbyLevel,
	MultiLevel,
	CooperationLevel,
	SingleLevel,
	TrainingLevel,
	FarmingLevel
};