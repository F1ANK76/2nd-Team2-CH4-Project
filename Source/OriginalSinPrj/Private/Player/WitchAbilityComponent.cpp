// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WitchAbilityComponent.h"
#include "Player/Abilies/BaseWitchAbility.h"
#include "Player/BaseWitch.h"
#include "GameFramework/CharacterMovementComponent.h"

UWitchAbilityComponent::UWitchAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UWitchAbilityComponent::CheckMoveable(const FVector2D& Value)
{
	if (!IsValid(ParentMovementComp))
	{
		return;
	}

	/*if (ParentMovementComp->IsFalling())
	{
		return;
	}*/

	if (!bIsMoveable)
	{
		return;
	}

	if (!IsValid(MoveAbility))
	{
		MoveAbility = SpawnAbility(MoveAbilityClass);
	}
	
	Direction = Value;
	//ParentWitch->SetWitchDirection(Direction); // 
	CurrentAbility = MoveAbility;
	ExcuteCurrentAbility(Value);
}

void UWitchAbilityComponent::CheckAttackable(const EAttackType AttackType)
{
	if (!bIsAttackable)
	{
		return;
	}

	bIsMoveable = false;
	bIsAttackable = false;
	bIsJumpable = false;

	switch (AttackType)
	{
	case EAttackType::NormalAttack:
		ApplyNormalAttack();
		break;

	case EAttackType::SpecialAttack:
		ApplySpecialAttack();
		break;

	case EAttackType::JumpAttack:
		ApplyJumpAttack();
		break;

	default:
		checkNoEntry();
		break;
	}
}

void UWitchAbilityComponent::CheckSkillAttackable(int32 SkillNum)
{
	if (!bIsAttackable)
	{
		return;
	}

	if (CurrentMana <= SkillNum)
	{
		return;
	}

	bIsMoveable = false;
	bIsAttackable = false;
	bIsJumpable = false;

	ApplySkillAttack(SkillNum);
}

void UWitchAbilityComponent::CheckJumpable()
{
	if (!bIsJumpable)
	{
		return;
	}

	if (!IsValid(JumpAbility))
	{
		JumpAbility = SpawnAbility(JumpAbilityClass);
	}

	CurrentAbility = JumpAbility;
	ExcuteCurrentAbility(Direction);
}

void UWitchAbilityComponent::CheckHitable(const FVector& ComparePos)
{
	bool bIsEqualDirection = CheckHittedDirection(ComparePos);

	if (IsValid(CurrentAbility))
	{
		if (CurrentAbility == RollAbility)
		{
			UE_LOG(LogTemp, Warning, TEXT("Current Ability is Roll"));
			return;
		}

		if (CurrentAbility == GuardAbility)
		{
			if (bIsEqualDirection)
			{
				UE_LOG(LogTemp, Warning, TEXT("Current Ability is Guard And Compare Direction is Equal"));
				return;
			}

			GuardAbility->UndoAbility();
		}
	}

	if (!IsValid(HitAbility))
	{
		HitAbility = SpawnAbility(HitAbilityClass);
	}

	CurrentAbility = HitAbility;

	if (bIsEqualDirection)
	{
		ExcuteCurrentAbility(-Direction);
	}
	else
	{
		ExcuteCurrentAbility(Direction);
	}
}

bool UWitchAbilityComponent::CheckHittedDirection(const FVector& HitActorPos)
{
	FVector HittedDirection = HitActorPos - ParentWitch->GetActorLocation();

	if (HittedDirection.Y > 0)
	{
		if (!bIsLeft)
		{
			return true;
		}

		return false;
	}
	else
	{
		if (bIsLeft)
		{
			return true;
		}

		return false;
	}
}

void UWitchAbilityComponent::ResponseEndAttack()
{
	bIsMoveable = true;
	bIsAttackable = true;
	bIsJumpable = true;
}

void UWitchAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GetOwner()))
	{
		ParentWitch = Cast<ABaseWitch>(GetOwner());
	}

	checkf(IsValid(ParentWitch), TEXT("Ability Component : Parent is invalid. Parent == nullptr || Not BaseWitch type"));

	ParentMovementComp = ParentWitch->GetCharacterMovement();
}

ABaseWitchAbility* UWitchAbilityComponent::SpawnAbility(UClass* TargetClass)
{
	if (!IsValid(ParentWitch))
	{
		if (IsValid(GetOwner()))
		{
			ParentWitch = Cast<ABaseWitch>(GetOwner());
		}
		else
		{
			return nullptr;
		}
	}

	if (!IsValid(TargetClass))
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParam;
	FVector SpawnPos = ParentWitch->GetActorLocation();
	FRotator SpawnRot = ParentWitch->GetActorRotation();

	TempAbility = GetWorld()->SpawnActor<ABaseWitchAbility>(TargetClass, SpawnPos, SpawnRot, SpawnParam);
	TempAbility->AttachToActor(ParentWitch, FAttachmentTransformRules::KeepWorldTransform);
	TempAbility->InitAbility(ParentWitch);

	return TempAbility;
}

void UWitchAbilityComponent::ExcuteCurrentAbility(const FVector2D& DirectionVector)
{
	if (!IsValid(CurrentAbility))
	{
		return;
	}

	CurrentAbility->ExcuteAbility(DirectionVector);
	AddLastAbilityToArray();
	ActiveTimer();
}

void UWitchAbilityComponent::ActiveTimer()
{
	GetWorld()->GetTimerManager().SetTimer(BufferTimer, this, &ThisClass::ClearLastAbilities, BufferActiveTime, false);
}

void UWitchAbilityComponent::AddLastAbilityToArray()
{
	LastAbilities.Add(CurrentAbility);
	LastAbilities.Insert(CurrentAbility, 0);
	RemoveOldAbilityFromArray();
}

void UWitchAbilityComponent::RemoveOldAbilityFromArray()
{
	if (LastAbilities.Num() > 5)
	{
		LastAbilities.RemoveAt(5);
	}
}

void UWitchAbilityComponent::ClearLastAbilities()
{
	LastAbilities.Empty();
	CurrentAbility = nullptr;
}

void UWitchAbilityComponent::ApplyNormalAttack()
{
	if (LastAbilities.IsEmpty())
	{
		if (!IsValid(NormalAttackAbility))
		{
			NormalAttackAbility = SpawnAbility(NormalAttackAbilityClass);
		}

		CurrentAbility = NormalAttackAbility;
	}
	else
	{
		int32 LastIndex = LastAbilities.Num() - 1;
		EAbilityType LastAbilityType = LastAbilities[LastIndex]->GetAbilityType();

		if (LastAbilityType == EAbilityType::MoveAbility || LastAbilityType == EAbilityType::JumpAbility)
		{
			CheckDirection();

			if (DirectionType == EDirectionType::Up)
			{
				if (!IsValid(UpperAttackAbility))
				{
					UpperAttackAbility = SpawnAbility(UpperAttackAbilityClass);
				}
				CurrentAbility = UpperAttackAbility;
			}
			else if (DirectionType == EDirectionType::Down)
			{
				if (!IsValid(LowerAttackAbility))
				{
					LowerAttackAbility = SpawnAbility(LowerAttackAbilityClass);
				}
				CurrentAbility = LowerAttackAbility;
			}
			else
			{
				if (!IsValid(DashAttackAbility))
				{
					DashAttackAbility = SpawnAbility(DashAttackAbilityClass);
				}
				CurrentAbility = DashAttackAbility;
			}
		}
		//else if (LastAbilityType == EAbilityType::JumpAbility)
		//{
		//	//Jump Attack...
		//}
		else
		{
			if (!IsValid(NormalAttackAbility))
			{
				NormalAttackAbility = SpawnAbility(NormalAttackAbilityClass);
			}

			CurrentAbility = NormalAttackAbility;
		}
	}

	ExcuteCurrentAbility(Direction);
}

void UWitchAbilityComponent::ApplySpecialAttack()
{
	if (LastAbilities.IsEmpty())
	{
		if (!IsValid(SpecialAttackAbility))
		{
			SpecialAttackAbility = SpawnAbility(SpecialAttackAbilityClass);
		}

		CurrentAbility = SpecialAttackAbility;
	}
	else
	{
		int32 LastIndex = LastAbilities.Num() - 1;
		EAbilityType LastAbilityType = LastAbilities[LastIndex]->GetAbilityType();

		if (LastAbilityType == EAbilityType::MoveAbility || LastAbilityType == EAbilityType::JumpAbility)
		{
			CheckDirection();

			if (DirectionType == EDirectionType::Up)
			{
				if (!IsValid(UppercutAttackAbility))
				{
					UppercutAttackAbility = SpawnAbility(UppercutAttackAbilityClass);
				}
				CurrentAbility = UppercutAttackAbility;
			}
			else if (DirectionType == EDirectionType::Down)
			{
				if (!IsValid(ChopAttackAbility))
				{
					ChopAttackAbility = SpawnAbility(ChopAttackAbilityClass);
				}
				CurrentAbility = ChopAttackAbility;
			}
			else
			{
				if (!IsValid(DropkickAttackAbility))
				{
					DropkickAttackAbility = SpawnAbility(DropKickAttackAbilityClass);
				}
				CurrentAbility = DropkickAttackAbility;
			}
		}
		//else if (LastAbilityType == EAbilityType::JumpAbility)
		//{
		//	//Jump Attack...
		//}
		else
		{
			if (!IsValid(SpecialAttackAbility))
			{
				SpecialAttackAbility = SpawnAbility(SpecialAttackAbilityClass);
			}

			CurrentAbility = SpecialAttackAbility;
		}
	}

	ExcuteCurrentAbility(Direction);
}

void UWitchAbilityComponent::ApplyJumpAttack()
{

}

void UWitchAbilityComponent::ApplySkillAttack(int32 SkillNum)
{
	switch (SkillNum)
	{
	case 0:
		if (!IsValid(Skill1Ability))
		{
			Skill1Ability = SpawnAbility(Skill1AbilityClass);
		}
		CurrentAbility = Skill1Ability;
		break;

	case 1:
		if (!IsValid(Skill2Ability))
		{
			Skill2Ability = SpawnAbility(Skill2AbilityClass);
		}
		CurrentAbility = Skill2Ability;
		break;

	case 2:
		if (!IsValid(Skill3Ability))
		{
			Skill3Ability = SpawnAbility(Skill3AbilityClass);
		}
		CurrentAbility = Skill3Ability;
		break;

	case 3:
		if (!IsValid(Skill4Ability))
		{
			Skill4Ability = SpawnAbility(Skill4AbilityClass);
		}
		CurrentAbility = Skill4Ability;
		break;

	case 4:
		if (!IsValid(Skill5Ability))
		{
			Skill5Ability = SpawnAbility(Skill5AbilityClass);
		}
		CurrentAbility = Skill5Ability;
		break;
	}

	CurrentMana -= SkillNum + 1;
	ExcuteCurrentAbility(Direction);
}

void UWitchAbilityComponent::CheckDirection()
{
	if (Direction.X > 0)
	{
		DirectionType = EDirectionType::Left;
		bIsLeft = true;
	}
	else if (Direction.X < 0)
	{
		DirectionType = EDirectionType::Right;
		bIsLeft = false;
	}
	else if (Direction.Y > 0)
	{
		DirectionType = EDirectionType::Up;
	}
	else if (Direction.Y < 0)
	{
		DirectionType = EDirectionType::Down;
	}
}



