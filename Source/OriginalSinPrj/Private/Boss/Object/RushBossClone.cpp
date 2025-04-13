// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Object/RushBossClone.h"

#include "Boss/BossController.h"
#include "Boss/BTTask/BTTask_RushBossCloneAttack.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

ARushBossClone::ARushBossClone()
{
	PrimaryActorTick.bCanEverTick = true;

	ZOffset = 100.0f;
	AcceptableDistance = 300.0f;
	RushSpeed = 12000.0f;
	AttackDuration = 2.0f;
	bIsRushing = false;
	bHasArrived = false;

	bReplicates = true;
	SetReplicateMovement(true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->Deactivate();
}

void ARushBossClone::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()) return;

	bIsActivate = false;
	MulticastSetActive(bIsActivate);
}

void ARushBossClone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority()) return;

	if (bIsRushing && !bHasArrived)
	{
		FVector NewLocation = GetActorLocation() + TargetDirection * RushSpeed * DeltaTime;
		SetActorLocation(NewLocation);

		CheckArrival();
	}
}

void ARushBossClone::OnPooledObjectSpawn_Implementation()
{
	if (!HasAuthority()) return;
	
	bIsActivate = true;
	MulticastSetActive(bIsActivate);
	
	ABossController* BossController = Cast<ABossController>(
		UGameplayStatics::GetActorOfClass(this, ABossController::StaticClass()));
	if (IsValid(BossController) && IsValid(BossController->GetPawn()))
	{
		InitializeClone(BossController->GetTargetPlayerPawnLocation());
	}
}

void ARushBossClone::OnPooledObjectReset_Implementation()
{
	if (!HasAuthority()) return;
	
	bIsActivate = false;
	MulticastSetActive(bIsActivate);
	
	bIsRushing = false;
	bHasArrived = false;

	TargetLocation = FVector::ZeroVector;
	TargetDirection = FVector::ZeroVector;
}

void ARushBossClone::MulticastSetActive_Implementation(bool bIsActive)
{
	SetActorHiddenInGame(!bIsActive);
}

void ARushBossClone::MulticastPlayCloneAttackMontage()
{
	if (CloneAttackMontage && GetNetMode() != NM_DedicatedServer)
	{
		PlayAnimMontage(CloneAttackMontage);
	}
}

void ARushBossClone::InitializeClone(const FVector& InTargetLocation)
{
	if (!HasAuthority()) return;

	TargetLocation = InTargetLocation + FVector(0, 0, ZOffset);
	TargetDirection = (TargetLocation - GetActorLocation()).GetSafeNormal();
	SetFacingDirection();

	bIsRushing = true;
	bHasArrived = false;
}

void ARushBossClone::CheckArrival()
{
	if (!HasAuthority()) return;

	float DistanceToTarget = FVector::Dist(GetActorLocation(), TargetLocation);
	if (DistanceToTarget <= AcceptableDistance)
	{
		bIsRushing = false;
		bHasArrived = true;

		FVector OffsetVector = -TargetDirection * AcceptableDistance;
		FVector FinalLocation = TargetLocation + OffsetVector;
		FinalLocation.Z += ZOffset;
		SetActorLocation(FinalLocation);
		
		StartAttack();
	}
}

void ARushBossClone::StartAttack()
{
	if (!HasAuthority()) return;

	//애니메이션

	//데미지

	//비활성화 타이머
	GetWorldTimerManager().SetTimer(
		DisappearTimerHandle,
		this,
		&ARushBossClone::FinishAttack,
		AttackDuration,
		false);
}

void ARushBossClone::FinishAttack()
{
	if (!HasAuthority()) return;
	
	IBossPoolableActorInterface::Execute_OnPooledObjectReset(this);
}

void ARushBossClone::SetFacingDirection()
{
	if (!HasAuthority()) return;
	
	UE_LOG(LogTemp, Warning, TEXT("Clone: TargetLocation = %s"), *TargetLocation.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Clone: MyLocation = %s"), *GetActorLocation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Clone: TargetDirection = %s"), *TargetDirection.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Clone: Yaw = %f"), TargetDirection.Rotation().Yaw);
	if (!TargetDirection.IsNearlyZero())
	{
		FRotator LookAtRotation = TargetDirection.Rotation();
		SetActorRotation(FRotator(0, LookAtRotation.Yaw, 0));
	}
}

void ARushBossClone::PlayCloneAttackMontage()
{
	if (HasAuthority())
	{
		MulticastPlayCloneAttackMontage();
	}
}
