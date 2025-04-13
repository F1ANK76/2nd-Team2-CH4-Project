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
	Attack,
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
enum class EAttackType : uint8
{
	Melle = 0,
	Spawn,
	Skill
};

UENUM(BlueprintType)
enum class EPivotType : uint8
{
	Body = 0,
	Head,
	Foot
};

UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	Normal = 0,
	VisibleWaitOnly,
	MoveWaitOnly,
	OverlapWaitOnly,
	VisibleAndMoveWait,
	VisibleAndOverlapWait,
	MoveAndOverlapWait
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

UENUM(BlueprintType)
enum class EEffectVisibleType : uint8
{
	Left = 0,
	Right,
	Both
};

UCLASS()
class ORIGINALSINPRJ_API UWitchTypes : public UObject
{
	GENERATED_BODY()
};
