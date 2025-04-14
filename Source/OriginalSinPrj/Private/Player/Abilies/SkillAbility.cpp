// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/SkillAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"
#include "Player/Projectile/BaseProjectile.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


ASkillAbility::ASkillAbility() : Super()
{
	WitchSelfEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Witch Effect"));
	SetRootComponent(WitchSelfEffect);
}

void ASkillAbility::ExcuteSkillAttack(const FAbilityDataBuffer& Buffer)
{
	Super::ExcuteSkillAttack(Buffer);

	GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
	CurrentSpawnCount = 0;
	AddedLocation = OriginAdded;

	switch (EffectPivot)
	{
	case EPivotType::Head:
		SetActorLocation(Buffer.ParentWitch->GetHeadLocation());
		break;

	case EPivotType::Body:
		SetActorLocation(Buffer.ParentWitch->GetActorLocation());
		break;

	case EPivotType::Foot:
		SetActorLocation(Buffer.ParentWitch->GetFootLocation());
		break;
	}

	PlaySelfEffect();
	ApplySpawnDelayTimer(Buffer);
	GetWorld()->GetTimerManager().SetTimer(EffectTimer, this, &ThisClass::StopSelfEffect, EffectKeepTime, false);
}

void ASkillAbility::UndoSkillAttack(const FAbilityDataBuffer& Buffer)
{
	Super::UndoSkillAttack(Buffer);
	StopSelfEffect();
	UndoSpawnAttack(Buffer);
}

void ASkillAbility::PlaySelfEffect_Implementation()
{
	if (IsValid(WitchSelfEffect))
	{
		WitchSelfEffect->Activate(true);
	}
}

void ASkillAbility::StopSelfEffect_Implementation()
{
	if (IsValid(WitchSelfEffect)) 
	{
		WitchSelfEffect->Deactivate();
	}
}

void ASkillAbility::BeginPlay()
{
	Super::BeginPlay();
	StopSelfEffect();
	OriginAdded = AddedLocation;
}

bool ASkillAbility::CheckExcuteable(FAbilityDataBuffer& Buffer)
{
	if (Buffer.NeedMana < ConsumeMana)
	{
		return false;
	}

	return Super::CheckExcuteable(Buffer);
}

void ASkillAbility::ApplySpawnDelayTimer(const FAbilityDataBuffer& Buffer)
{
	if (FMath::IsNearlyZero(SpawnDelayTime))
	{
		ExcuteNonDelaySpawn(Buffer);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnTimer, FTimerDelegate::CreateLambda([&]()
			{
				ExcuteSpawn(Buffer);

				if (CurrentSpawnCount >= SpawnCount)
				{
					GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
				}
			}), SpawnDelayTime, true);
	}
}

void ASkillAbility::ExcuteSpawn(const FAbilityDataBuffer& Buffer)
{
	if (bUseRandomPos)
	{
		CalculateRandomDirection();
	}

	ExcuteSpawnAttack(Buffer);
	++CurrentSpawnCount;

	if (bSpawnContinued)
	{
		CalculateNextSpawnPos();
	}
}

void ASkillAbility::ExcuteNonDelaySpawn(const FAbilityDataBuffer& Buffer)
{
	if (SpawnCount <= 0)
	{
		return;
	}

	for (int32 i = 0; i < SpawnCount; ++i)
	{
		ExcuteSpawn(Buffer);
	}
}

void ASkillAbility::CalculateRandomDirection()
{
	MoveDirection.X = 0;
	MoveDirection.Y = FMath::RandRange(-1, 1);
	MoveDirection.Z = FMath::RandRange(-1, 1);

	if (FMath::IsNearlyZero(MoveDirection.Y) && FMath::IsNearlyZero(MoveDirection.Z))
	{
		CalculateRandomDirection();
	}
}

void ASkillAbility::CalculateNextSpawnPos()
{
	if (AddedLocation.Y < 0)
	{
		AddedLocation -= OriginAdded;
	}
	else
	{
		AddedLocation += OriginAdded;
	}
}
