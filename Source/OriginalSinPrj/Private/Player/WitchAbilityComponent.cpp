// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WitchAbilityComponent.h"
#include "Player/Abilies/BaseWitchAbility.h"
#include "Player/BaseWitch.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/Struct/CharacterStateBuffer.h"

UWitchAbilityComponent::UWitchAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UWitchAbilityComponent::CallMove(const FVector2D& Value)
{
	if (!IsValid(ParentMovementComp))
	{
		return;
	}

	AbilityBuffer.MoveValueVector = Value;

	if (!IsValid(MoveAbility))
	{
		MoveAbility = SpawnAbility(MoveAbilityClass);
	}

	AbilityBuffer.CurrentAbility = MoveAbility;
	ExcuteCurrentAbility();

	if (AbilityBuffer.CurrentAbility == GuardAbility)
	{
		CallRoll(Value);
	}
}

void UWitchAbilityComponent::CallNormalAttack()
{
	if (!AbilityBuffer.LastAbilities.IsEmpty())
	{
		if (AbilityBuffer.LastAbilities[0] == JumpAbility)
		{
			CallNormalAttackAtJump();
			return;
		}
	}

	if (AbilityBuffer.ComandDirection != EDirectionType::None)
	{
		CallNormalAttackAtMove();
		return;
	}

	if (!IsValid(NormalAttackAbility))
	{
		NormalAttackAbility = SpawnAbility(NormalAttackAbilityClass);
	}

	AbilityBuffer.CurrentAbility = NormalAttackAbility;
	ExcuteCurrentAbility();
}

void UWitchAbilityComponent::CallSpecialAttack()
{
	if (!AbilityBuffer.LastAbilities.IsEmpty())
	{
		if (AbilityBuffer.LastAbilities[0] == JumpAbility)
		{
			CallSpecialAttackAtJump();
			return;
		}
	}

	if (AbilityBuffer.ComandDirection != EDirectionType::None)
	{
		CallSpecialAttackAtMove();
		return;
	}

	if (!IsValid(SpecialAttackAbility))
	{
		SpecialAttackAbility = SpawnAbility(SpecialAttackAbilityClass);
	}

	AbilityBuffer.CurrentAbility = SpecialAttackAbility;
	ExcuteCurrentAbility();
}

void UWitchAbilityComponent::CallSkillAttack(int32 SkillNum)
{
	switch (SkillNum)
	{
	case 0:
		if (!IsValid(Skill1Ability))
		{
			Skill1Ability = SpawnAbility(Skill1AbilityClass);
		}
		AbilityBuffer.CurrentAbility = Skill1Ability;
		break;

	case 1:
		if (!IsValid(Skill2Ability))
		{
			Skill2Ability = SpawnAbility(Skill2AbilityClass);
		}
		AbilityBuffer.CurrentAbility = Skill2Ability;
		break;

	case 2:
		if (!IsValid(Skill3Ability))
		{
			Skill3Ability = SpawnAbility(Skill3AbilityClass);
		}
		AbilityBuffer.CurrentAbility = Skill3Ability;
		break;

	case 3:
		if (!IsValid(Skill4Ability))
		{
			Skill4Ability = SpawnAbility(Skill4AbilityClass);
		}
		AbilityBuffer.CurrentAbility = Skill4Ability;
		break;

	case 4:
		if (!IsValid(Skill5Ability))
		{
			Skill5Ability = SpawnAbility(Skill5AbilityClass);
		}
		AbilityBuffer.CurrentAbility = Skill5Ability;
		break;
	}
	AbilityBuffer.NeedMana = SkillNum + 1;
	ExcuteCurrentAbility();
}

void UWitchAbilityComponent::CallJump()
{
	if (!IsValid(JumpAbility))
	{
		JumpAbility = SpawnAbility(JumpAbilityClass);
	}

	AbilityBuffer.CurrentAbility = JumpAbility;
	ExcuteCurrentAbility();
}

void UWitchAbilityComponent::CallHit(AActor* DamageCauser, float DamageValue)
{
	AbilityBuffer.DamageCauser = DamageCauser;
	AbilityBuffer.AddedGuage = DamageValue;

	if (!IsValid(HitAbility))
	{
		HitAbility = SpawnAbility(HitAbilityClass);
	}
	
	AbilityBuffer.CurrentAbility = HitAbility;
	ExcuteCurrentAbility();
}

void UWitchAbilityComponent::CallBeginGuard()
{
	if (!IsValid(GuardAbility))
	{
		GuardAbility = SpawnAbility(GuardAbilityClass);
	}

	AbilityBuffer.CurrentAbility = GuardAbility;
	ExcuteCurrentAbility();
}

void UWitchAbilityComponent::CallKeepGuard()
{
	if (!IsValid(AbilityBuffer.CurrentAbility))
	{
		return;
	}

	if (AbilityBuffer.CurrentAbility == GuardAbility)
	{
		ActiveTimer();
	}
}

void UWitchAbilityComponent::CallEndGuard()
{
	if (!IsValid(AbilityBuffer.CurrentAbility))
	{
		return;
	}

	if (AbilityBuffer.CurrentAbility == GuardAbility)
	{
		ResponseEndAnim();
	}
}

void UWitchAbilityComponent::CallTaunt()
{
	if (!IsValid(TauntAbility))
	{
		TauntAbility = SpawnAbility(TauntAbilityClass);
	}

	AbilityBuffer.CurrentAbility = TauntAbility;
	ExcuteCurrentAbility();
}

void UWitchAbilityComponent::CallRoll(const FVector2D& DirectionVector)
{
	if (!IsValid(RollAbility))
	{
		RollAbility = SpawnAbility(RollAbilityClass);
	}

	AbilityBuffer.CurrentAbility = RollAbility;
	ExcuteCurrentAbility();
}

void UWitchAbilityComponent::ResponseEndAnim()
{
	bIsPlayingAnim = false;
	//UE_LOG(LogTemp, Warning, TEXT("%s : Response End Anim"), *AbilityBuffer.ParentWitch->GetName());
	if (IsValid(AbilityBuffer.CurrentAbility))
	{
		AbilityBuffer.CurrentAbility->UndoAbility(AbilityBuffer);
		//UE_LOG(LogTemp, Warning, TEXT("%s : Undo Ability %s"), *AbilityBuffer.ParentWitch->GetName(), *AbilityBuffer.CurrentAbility->GetName());
		if (AbilityBuffer.CurrentAbility == JumpAbility)
		{
			//UE_LOG(LogTemp, Warning, TEXT("%s : Undo Target Ability is Jump Ability"), *AbilityBuffer.ParentWitch->GetName());
			GetWorld()->GetTimerManager().ClearTimer(BufferTimer);
			ClearLastAbilities();
		}
	}

	if (GetWorld()->GetTimerManager().TimerExists(BufferTimer))
	{
		GetWorld()->GetTimerManager().UnPauseTimer(BufferTimer);
	}
}

void UWitchAbilityComponent::PauseBufferTimer()
{
	if (GetWorld()->GetTimerManager().TimerExists(BufferTimer))
	{
		GetWorld()->GetTimerManager().PauseTimer(BufferTimer);
	}
	else
	{
		bIsPlayingAnim = true;
	}
}

void UWitchAbilityComponent::AddCurrentMana(float Value)
{
	AbilityBuffer.CurrentMana = FMath::Clamp(AbilityBuffer.CurrentMana + Value, 0, AbilityBuffer.MaxMana);
}

void UWitchAbilityComponent::ResetAbility()
{
	if (IsValid(AbilityBuffer.CurrentAbility))
	{
		AbilityBuffer.CurrentAbility->UndoAbility(AbilityBuffer);
	}

	GetWorld()->GetTimerManager().ClearTimer(BufferTimer);
	ClearLastAbilities();

	AbilityBuffer.bIsInAir = false;
	AbilityBuffer.bIsJumpable = true;
	AbilityBuffer.bIsLeft = false;
	AbilityBuffer.bIsMoveable = true;
	AbilityBuffer.bIsUseable = true;
}

void UWitchAbilityComponent::OnChangedCharacterState(const FCharacterStateBuffer& Buffer)
{
	AbilityBuffer.CurrentMana = Buffer.CurrentMana;
	AbilityBuffer.MaxMana = Buffer.MaxMana;
	AbilityBuffer.KnockGuage = Buffer.AirbornePercent;
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
	AbilityBuffer.ParentWitch = ParentWitch;
	AbilityBuffer.MovementComp = ParentMovementComp;

	if (ParentWitch->HasAuthority())
	{
		ParentWitch->OnChangedState.AddDynamic(this, &ThisClass::OnChangedCharacterState);
	}
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
	TempAbility->InitAbility();

	return TempAbility;
}

void UWitchAbilityComponent::ExcuteCurrentAbility()
{
	if (!IsValid(AbilityBuffer.CurrentAbility))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Current Ability is invalid"));
		return;
	}

	bool bIsExcuteable = AbilityBuffer.CurrentAbility->ExcuteAbility(AbilityBuffer);

	if (bIsExcuteable)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Current Ability Excute Sucessed. %s"), *AbilityBuffer.CurrentAbility->GetName());
		if (AbilityBuffer.CurrentAbility != MoveAbility)
		{
			AbilityBuffer.ComandDirection = EDirectionType::None;
		}

		AddLastAbilityToArray();
		ActiveTimer();
	}
	else
	{
		if (!AbilityBuffer.LastAbilities.IsEmpty())
		{
			AbilityBuffer.CurrentAbility = AbilityBuffer.LastAbilities[0];
			//UE_LOG(LogTemp, Warning, TEXT("Current Ability Excute Fail. Back Last %s"), *AbilityBuffer.CurrentAbility->GetName());
		}
	}
}

void UWitchAbilityComponent::ActiveTimer()
{
	if (!GetWorld()->GetTimerManager().TimerExists(BufferTimer))
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s : Timer is invalid. "), *AbilityBuffer.ParentWitch->GetName());
		GetWorld()->GetTimerManager().SetTimer(BufferTimer, this, &ThisClass::ClearLastAbilities, BufferActiveTime, false);

		if (bIsPlayingAnim)
		{
			//UE_LOG(LogTemp, Warning, TEXT("%s : Animation is Playing. Timer Pause. "), *AbilityBuffer.ParentWitch->GetName());
			GetWorld()->GetTimerManager().PauseTimer(BufferTimer);
		}

		return;
	}

	if (!GetWorld()->GetTimerManager().IsTimerPaused(BufferTimer))
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s : Timer is not paused. Set Timer. "), *AbilityBuffer.ParentWitch->GetName());
		GetWorld()->GetTimerManager().SetTimer(BufferTimer, this, &ThisClass::ClearLastAbilities, BufferActiveTime, false);
	}
}

void UWitchAbilityComponent::AddLastAbilityToArray()
{
	AbilityBuffer.LastAbilities.Insert(AbilityBuffer.CurrentAbility, 0);
	RemoveOldAbilityFromArray();
}

void UWitchAbilityComponent::RemoveOldAbilityFromArray()
{
	if (AbilityBuffer.LastAbilities.Num() > 5)
	{
		AbilityBuffer.LastAbilities.RemoveAt(5);
	}
}

void UWitchAbilityComponent::ClearLastAbilities()
{
	//UE_LOG(LogTemp, Warning, TEXT("%s : Clear Last Ability Array"), *AbilityBuffer.ParentWitch->GetName());
	AbilityBuffer.LastAbilities.Empty();
	AbilityBuffer.CurrentAbility = nullptr;
	AbilityBuffer.ComandDirection = EDirectionType::None;
	AbilityBuffer.MoveValueVector = FVector2D::ZeroVector;
}

void UWitchAbilityComponent::CallNormalAttackAtMove()
{
	switch (AbilityBuffer.ComandDirection)
	{
	case EDirectionType::Up:

		if (!IsValid(UpperAttackAbility))
		{
			UpperAttackAbility = SpawnAbility(UpperAttackAbilityClass);
		}

		AbilityBuffer.CurrentAbility = UpperAttackAbility;
		break;

	case EDirectionType::Down:

		if (!IsValid(LowerAttackAbility))
		{
			LowerAttackAbility = SpawnAbility(LowerAttackAbilityClass);
		}

		AbilityBuffer.CurrentAbility = LowerAttackAbility;
		break;

	case EDirectionType::Left:
	case EDirectionType::Right:

		if (!IsValid(DashAttackAbility))
		{
			DashAttackAbility = SpawnAbility(DashAttackAbilityClass);
		}

		AbilityBuffer.CurrentAbility = DashAttackAbility;
		break;
	}

	ExcuteCurrentAbility();
}

void UWitchAbilityComponent::CallNormalAttackAtJump()
{
	UE_LOG(LogTemp, Warning, TEXT("Call Normal Jump Attack"));
}

void UWitchAbilityComponent::CallSpecialAttackAtMove()
{
	switch (AbilityBuffer.ComandDirection)
	{
	case EDirectionType::Up:

		if (!IsValid(UppercutAttackAbility))
		{
			UppercutAttackAbility = SpawnAbility(UppercutAttackAbilityClass);
		}

		AbilityBuffer.CurrentAbility = UppercutAttackAbility;
		break;

	case EDirectionType::Down:

		if (!IsValid(ChopAttackAbility))
		{
			ChopAttackAbility = SpawnAbility(ChopAttackAbilityClass);
		}

		AbilityBuffer.CurrentAbility = ChopAttackAbility;
		break;

	case EDirectionType::Left:
	case EDirectionType::Right:

		if (!IsValid(DropkickAttackAbility))
		{
			DropkickAttackAbility = SpawnAbility(DropKickAttackAbilityClass);
		}

		AbilityBuffer.CurrentAbility = DropkickAttackAbility;
		break;
	}

	ExcuteCurrentAbility();
}

void UWitchAbilityComponent::CallSpecialAttackAtJump()
{
	UE_LOG(LogTemp, Warning, TEXT("Call Special Jump Attack"));
}




