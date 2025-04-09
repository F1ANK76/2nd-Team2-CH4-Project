// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WitchTypes.generated.h"

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	None = 0,
	Witch1,
	Witch2,
	Witch3
};

UENUM(BlueprintType)
enum class EWitchStateType : uint8
{
	Idle = 0,
	Move,
	Jump,
	Hit,
	Guard,
	Roll,
	Taunt,
	NormalAttack,
	SpecialAttack,
	SkillAttack
};

UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	None = 0,
	MoveAbility,
	JumpAbility,
	GuardAbility,
	RollAbility,
	TauntAbility,
	HitAbility,
	NormalAttackAbility,
	UpperAttackAbility,
	LowerAttackAbility,
	DashAttackAbility,
	SpecialAttackAbility,
	UppercutAttackAbility,
	ChopAttackAbility,
	DropkickAttackAbility,
	Skill1Ability,
	Skill2Ability,
	Skill3Ability,
	Skill4Ability,
	Skill5Ability
};

UENUM(BlueprintType)
enum class EDirectionType : uint8
{
	None = 0,
	Left,
	Right,
	Up,
	Down
};

UCLASS()
class ORIGINALSINPRJ_API UWitchTypes : public UObject
{
	GENERATED_BODY()
};
