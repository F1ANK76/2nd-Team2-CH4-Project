// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossCharacter.h"
#include "Boss/BossController.h"
#include "Net/UnrealNetwork.h"
#include "Components/AudioComponent.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	MaxHP = 1000;
	CurrentHP = MaxHP;
	bIsDead = false;
	bIsInvincibility = false;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->SetupAttachment(RootComponent);
	
	AIControllerClass = ABossController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bReplicates = true;
	SetReplicateMovement(true);
}

float ABossCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (!HasAuthority()) return 0.0f;
	if (!IsValid(DamageCauser)) return 0.0f;
	if (bIsInvincibility) return 0.0f;

	float DamageApplied = FMath::Clamp(DamageAmount, 0.0f, CurrentHP);
	CurrentHP -= DamageAmount;

	OnBossTakeDamage.Broadcast(CurrentHP);
	
	return 0.0f;
}

void ABossCharacter::UpdateFacingDirection(APawn* ClosestPlayer)
{
	if (!IsValid(ClosestPlayer)) return;

	if (HasAuthority())
	{
		FVector BossLocation = GetActorLocation();
		FVector PlayerLocation = ClosestPlayer->GetActorLocation();

		float Direction = PlayerLocation.Y - BossLocation.Y;

		SetFacingDirection(Direction);
	}
}

void ABossCharacter::SetFacingDirection(float Direction)
{
	if (Direction < 0)
	{
		//왼쪽 방향
		SetActorRotation(FRotator(0.0f, -60.0f, 0.0f));
	}
	else
	{
		//오른쪽 방향
		SetActorRotation(FRotator(0.0f, 60.0f, 0.0f));
	}
}

void ABossCharacter::MulticastPlayStartBattleMontage_Implementation()
{
	if (IsValid(StartBattleMontage))
	{
		PlayAnimMontage(StartBattleMontage);
	}
}

void ABossCharacter::MulticastPlayDeathMontage_Implementation()
{
	if (IsValid(DeathMontage))
	{
		PlayAnimMontage(DeathMontage);
	}
}

void ABossCharacter::MulticastPlayRangeAttackMontage_Implementation()
{
	if (IsValid(RangeAttackMontage))
	{
		PlayAnimMontage(RangeAttackMontage);
	}
}

void ABossCharacter::MulticastPlayAreaSpawnWeaponMontage_Implementation()
{
	if (IsValid(AreaSpawnWeaponMontage))
	{
		PlayAnimMontage(AreaSpawnWeaponMontage);
	}
}

void ABossCharacter::MulticastPlayRushBossAttackMontage_Implementation()
{
	if (IsValid(RushBossAttackMontage))
	{
		PlayAnimMontage(RushBossAttackMontage);
	}
}

void ABossCharacter::MulticastPlayHijackAttackMontage_Implementation()
{
	if (IsValid(HijackAttackMontage))
	{
		PlayAnimMontage(HijackAttackMontage);
	}
}

void ABossCharacter::MulticastPlayInstantDeathAttackMontage_Implementation()
{
	if (IsValid(InstantDeathAttackMontage))
	{
		PlayAnimMontage(InstantDeathAttackMontage);
	}
}

void ABossCharacter::MulticastPlayStartStunMontage_Implementation()
{
	if (IsValid(StartStunMontage))
	{
		PlayAnimMontage(StartStunMontage);
	}
}

void ABossCharacter::MulticastPlayEndStunMontage_Implementation()
{
	if (IsValid(EndStunMontage))
	{
		PlayAnimMontage(EndStunMontage);
	}
}

void ABossCharacter::MulticastPlayIndexAttackMontage_Implementation()
{
	if (IsValid(IndexAttackMontage))
	{
		PlayAnimMontage(IndexAttackMontage);
	}
}

void ABossCharacter::MulticastPlayKillAllPlayerAttackMontage_Implementation()
{
	if (IsValid(KillAllPlayerAttackMontage))
	{
		PlayAnimMontage(KillAllPlayerAttackMontage);
	}
}

void ABossCharacter::PlayStartBattleMontage()
{
	if (HasAuthority())
	{
		MulticastPlayStartBattleMontage();
	}
}

void ABossCharacter::PlayDeathMontage()
{
	if (HasAuthority())
	{
		MulticastPlayDeathMontage();
	}
}

void ABossCharacter::PlayStartStunMontage()
{
	if (HasAuthority())
	{
		MulticastPlayStartStunMontage();
	}
}

void ABossCharacter::PlayEndStunMontage()
{
	if (HasAuthority())
	{
		MulticastPlayEndStunMontage();
	}
}

void ABossCharacter::PlayRangeAttackMontage()
{
	if (HasAuthority())
	{
		MulticastPlayRangeAttackMontage();
	}
}

void ABossCharacter::PlayAreaSpawnWeaponMontage()
{
	if (HasAuthority())
	{
		MulticastPlayAreaSpawnWeaponMontage();
	}
}

void ABossCharacter::PlayRushBossAttackMontage()
{
	if (HasAuthority())
	{
		MulticastPlayRushBossAttackMontage();
	}
}

void ABossCharacter::PlayHijackAttackMontage()
{
	if (HasAuthority())
	{
		MulticastPlayHijackAttackMontage();
	}
}

void ABossCharacter::PlayInstantDeathAttackMontage()
{
	if (HasAuthority())
	{
		MulticastPlayInstantDeathAttackMontage();
	}
}

void ABossCharacter::PlayIndexAttackMontage()
{
	if (HasAuthority())
	{
		MulticastPlayIndexAttackMontage();
	}
}

void ABossCharacter::PlayKillAllPlayerAttackMontage()
{
	if (HasAuthority())
	{
		MulticastPlayKillAllPlayerAttackMontage();
	}
}
