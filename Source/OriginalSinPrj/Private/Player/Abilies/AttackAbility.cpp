// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilies/AttackAbility.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/AbilityDataBuffer.h"
#include "Player/Projectile/BaseProjectile.h"
#include "GameState/BaseGameState.h"

bool AAttackAbility::ExcuteAbility(FAbilityDataBuffer& Buffer)
{
	Super::ExcuteAbility(Buffer);

	if (!CheckExcuteable(Buffer))
	{
		return false;
	}

	Buffer.bIsJumpable = false;
	Buffer.bIsMoveable = false;
	Buffer.bIsUseable = false;

	ExcuteAttackByType(Buffer);

	Buffer.ParentWitch->SetWitchState(EWitchStateType::Attack);
	Buffer.ParentWitch->PlayAnimation(AbilityMontage);

	return true;
}

void AAttackAbility::UndoAbility(FAbilityDataBuffer& Buffer)
{
	Super::UndoAbility(Buffer);

	Buffer.ParentWitch->StopAnimation(AbilityMontage);

	Buffer.bIsJumpable = true;
	Buffer.bIsMoveable = true;
	Buffer.bIsUseable = true;

	UndoAttackByType(Buffer);
}

bool AAttackAbility::CheckExcuteable(FAbilityDataBuffer& Buffer)
{
	Super::CheckExcuteable(Buffer);

	if (!Buffer.bIsUseable)
	{
		return false;
	}

	return true;
}

void AAttackAbility::ExcuteAttackByType(FAbilityDataBuffer& Buffer)
{
	switch (AttackType)
	{
	case EAttackType::Melle:
		ExcuteMelleAttack(Buffer);
		break;

	case EAttackType::Spawn:
		ExcuteSpawnAttack(Buffer);
		break;

	case EAttackType::Skill:
		ExcuteSkillAttack(Buffer);
		break;

	default:
		checkNoEntry();
	}
}

void AAttackAbility::UndoAttackByType(const FAbilityDataBuffer& Buffer)
{
	switch (AttackType)
	{
	case EAttackType::Melle:
		UndoMelleAttack(Buffer);
		break;

	case EAttackType::Spawn:
		UndoSpawnAttack(Buffer);
		break;

	case EAttackType::Skill:
		UndoSkillAttack(Buffer);
		break;

	default:
		checkNoEntry();
	}
}

void AAttackAbility::ExcuteMelleAttack(const FAbilityDataBuffer& Buffer)
{
	CheckIsPlayWitchEffect(Buffer.ParentWitch, true);
	Buffer.ParentWitch->PlayMelleAttack(MelleType, DefaultDamage + Buffer.AddedDamage);
}

void AAttackAbility::ExcuteSpawnAttack(const FAbilityDataBuffer& Buffer)
{
	CheckProjectilePool();

	if (IsValid(ProjectileObj))
	{
		CalculateProjectilePos(Buffer.ParentWitch);
		MoveDirection.Y = FMath::Abs(MoveDirection.Y);
		AddedLocation.Y = FMath::Abs(AddedLocation.Y);

		if (Buffer.bIsLeft)
		{
			ProjectileObj->SetActorRotation(FRotator(0, -90, 0));
			AddedLocation.Y *= -1;
			MoveDirection.Y *= -1;
		}
		else
		{
			ProjectileObj->SetActorRotation(FRotator(0, 90, 0));
			
		}

		SpawnLocation += AddedLocation;

		UpdateProjectileData(Buffer);

		ProjectileObj->SetActorLocation(SpawnLocation);
		ProjectileObj->ActiveProjectile(ProjectileData);

		ActiveProjectilePool.Add(ProjectileObj);
		ProjectilePool.Remove(ProjectileObj);
	}

	CheckIsPlayWitchEffect(Buffer.ParentWitch, true);
}

void AAttackAbility::ExcuteSkillAttack(FAbilityDataBuffer& Buffer)
{
	CheckIsPlayWitchEffect(Buffer.ParentWitch, true);
}

void AAttackAbility::UndoMelleAttack(const FAbilityDataBuffer& Buffer)
{
	CheckIsPlayWitchEffect(Buffer.ParentWitch, false);
	Buffer.ParentWitch->StopMelleAttack();
}

void AAttackAbility::UndoSpawnAttack(const FAbilityDataBuffer& Buffer)
{
	CheckIsPlayWitchEffect(Buffer.ParentWitch, false);

	if (ActiveProjectilePool.IsEmpty())
	{
		return;
	}

	for (ABaseProjectile* Element : ActiveProjectilePool)
	{
		Element->DeactiveProjectile();

		ProjectilePool.Add(Element);
	}

	ActiveProjectilePool.Empty();
}

void AAttackAbility::UndoSkillAttack(const FAbilityDataBuffer& Buffer)
{
	CheckIsPlayWitchEffect(Buffer.ParentWitch, false);
}

void AAttackAbility::UpdateProjectileData(const FAbilityDataBuffer& Buffer)
{
	ProjectileData.ParentWitch = Buffer.ParentWitch;
	ProjectileData.AttackDamage = DefaultDamage + Buffer.AddedDamage;
	ProjectileData.AttackDelay = AttackDelayTime * (1 - (Buffer.AttackSpeed - 1));
	ProjectileData.MoveDelay = MoveDelayTime * (1 - (Buffer.AttackSpeed - 1));
	ProjectileData.VisibleDelay = VisibleDelayTime * (1 - (Buffer.AttackSpeed - 1));
	ProjectileData.DeactiveDelay = DeactiveDelayTime * (1 - (Buffer.AttackSpeed - 1));
	ProjectileData.MoveDirection = MoveDirection;
	ProjectileData.MoveSpeed = MoveSpeed * Buffer.AttackSpeed;
}

void AAttackAbility::CalculateProjectilePos(ABaseWitch* Parent)
{
	SpawnLocation = FVector::ZeroVector;

	switch (PivotType)
	{
	case EPivotType::Body:
		SpawnLocation += Parent->GetActorLocation();
		break;

	case EPivotType::Head:
		SpawnLocation += Parent->GetHeadLocation();
		break;

	case EPivotType::Foot:
		SpawnLocation += Parent->GetFootLocation();
		break;
	}
}

void AAttackAbility::CheckProjectilePool()
{
	if (ProjectilePool.IsEmpty())
	{
		SpawnProjectileObj();
		return;
	}
	
	bool bIsNotUseable = true;

	for (int32 i = 0; i < ProjectilePool.Num(); ++i)
	{
		bIsNotUseable = ProjectilePool[i]->GetIsActevated();

		if (!bIsNotUseable)
		{
			ProjectileObj = ProjectilePool[i];
			break;
		}
	}

	if (bIsNotUseable)
	{
		SpawnProjectileObj();
	}
}

void AAttackAbility::SpawnProjectileObj()
{
	if (IsValid(ProjectileClass))
	{
		ProjectileObj = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass);
		ProjectilePool.Add(ProjectileObj);
	}
}

void AAttackAbility::CheckIsPlayWitchEffect(ABaseWitch* Parent, bool bIsStart)
{
	if (bIsPlayWitchEffect)
	{
		if (bIsStart)
		{
			Parent->PlayEffect(MelleType);

			if (CheckValidOfGameState())
			{
				BaseGameState->PlayCharacterSound(Parent->GetAudioComponent(), Parent->GetAttackSoundType());
			}
		}
		else
		{
			Parent->StopEffect();

			if (CheckValidOfGameState())
			{
				BaseGameState->StopEffectSound(Parent->GetAudioComponent());
			}
		}
	}
}
