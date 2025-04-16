// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BuffComponent.h"
#include "Player/BaseWitch.h"

UBuffComponent::UBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBuffComponent::ApplyBuff(EBuffType TargetType)
{
	switch (TargetType)
	{
	case EBuffType::ManaUp:
		ApplyAddedManaUp();
		break;

	case EBuffType::AttackUp:
		ApplyAttackUp();
		break;

	case EBuffType::CircleUp:
		ApplyMaxManaUp();
		break;

	case EBuffType::LifePointUp:
		ApplyLifePointUp();
		break;

	case EBuffType::DefenseUp:
		ApplyDefenceUp();
		break;

	case EBuffType::AttackSpeedUp:
		ApplyAttackSpeedUp();
		break;

	case EBuffType::AvoidanceUp:
		ApplyAvoidenceUp();
		break;

	case EBuffType::AttackRangeUp:
		ApplyAttackRangeUp();
		break;

	case EBuffType::EnemyCircleDown:
		ApplyEnemyMaxManaDown();
		break;

	default:
		checkNoEntry();
	}
}

const FBuffDataBuffer& UBuffComponent::GetBuffData() const
{
	return BuffData;
}

void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GetOwner()))
	{
		ParentWitch = Cast<ABaseWitch>(GetOwner());
	}
}

void UBuffComponent::ApplyAttackUp()
{
	BuffData.AddedKnockGauge = AddedKnockGaugeValue;
}

void UBuffComponent::ApplyAttackRangeUp()
{
	BuffData.AddedAttackRange = AddedAttackRangeValue;
}

void UBuffComponent::ApplyAttackSpeedUp()
{
	BuffData.AddedAttackSpeed = AddedAttackSpeedValue;
}

void UBuffComponent::ApplyMaxManaUp()
{
	if (IsValid(ParentWitch))
	{
		ParentWitch->IncreaseCircle();
	}
}

void UBuffComponent::ApplyAddedManaUp()
{
	BuffData.AddedMana += AddedManaValue;
}

void UBuffComponent::ApplyDefenceUp()
{
	BuffData.AddedGuardPoint = AddedGuardPointValue;
}

void UBuffComponent::ApplyAvoidenceUp()
{
	BuffData.AddedAvoidRate = AddedAvoidRateValue;
}

void UBuffComponent::ApplyLifePointUp()
{
	if (IsValid(ParentWitch))
	{
		ParentWitch->IncreaseLifePoint();
	}
}

void UBuffComponent::ApplyEnemyMaxManaDown()
{
	if (IsValid(ParentWitch))
	{
		ParentWitch->DecreaseLifePoint();
	}
}
