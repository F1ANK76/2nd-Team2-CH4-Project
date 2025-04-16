// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Object/RushBossClone.h"

#include "Boss/BossController.h"
#include "Boss/BossCharacter.h"
#include "Boss/HijackBossController.h"
#include "Boss/BTTask/BTTask_RushBossCloneAttack.h"
#include "Boss/Object/DestructibleObject.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

ARushBossClone::ARushBossClone()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->Deactivate();
}

void ARushBossClone::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()) return;

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARushBossClone::OnBeginOverlap);
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
	if (!IsValid(BossController) && !IsValid(BossController->GetPawn())) return;
	
	AHijackBossController* HijackBossController = Cast<AHijackBossController>(
		UGameplayStatics::GetActorOfClass(this, AHijackBossController::StaticClass()));
	if (!IsValid(HijackBossController) && !IsValid(HijackBossController->GetPawn())) return;

	//납치패턴 보스가 활성화 상태일 경우, 거리 비교하여 가까운 플레이어 타겟팅
	//비활성화 상태일 경우, 일반보스의 타겟플레이어 타겟팅
	if (HijackBossController->GetBattleState())
	{
		InitializeClone(HijackBossController->GetTargetPlayerPawnLocation());
	}
	else
	{
		InitializeClone(BossController->GetTargetPlayerPawnLocation());
	}

	GetWorld()->GetTimerManager().SetTimer(
		LifeTimeTimerHandle,
		this,
		&ARushBossClone::OnPooledObjectReset_Implementation,
		LifeTime,
		false);
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

	GetWorld()->GetTimerManager().ClearTimer(LifeTimeTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(DisappearTimerHandle);
}

void ARushBossClone::MulticastSetActive_Implementation(bool bIsActive)
{
	SetActorHiddenInGame(!bIsActive);
	SphereComponent->SetActive(bIsActive);
}

void ARushBossClone::MulticastPlayCloneAttackMontage()
{
	if (CloneAttackMontage)
	{
		PlayAnimMontage(CloneAttackMontage);
	}
}

void ARushBossClone::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && OtherActor->ActorHasTag("Destructible"))
	{
		ABossController* BossController = Cast<ABossController>(
		UGameplayStatics::GetActorOfClass(this, ABossController::StaticClass()));
		ADestructibleObject* DestructibleObject = Cast<ADestructibleObject>(OtherActor);
		
		if (IsValid(BossController) && IsValid(DestructibleObject))
		{
			BossController->EnterToStunState();
		}
		IBossPoolableActorInterface::Execute_OnPooledObjectReset(this);
		IBossPoolableActorInterface::Execute_OnPooledObjectReset(Cast<ADestructibleObject>(DestructibleObject));
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
