// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Object/RushBossClone.h"

#include "Boss/BossController.h"
#include "Boss/BossCharacter.h"
#include "Boss/HijackBossController.h"
#include "Boss/Object/DestructibleObject.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BaseWitch.h"

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

	DamageArea = CreateDefaultSubobject<USphereComponent>(TEXT("DamageArea"));
	DamageArea->SetupAttachment(RootComponent);
	DamageArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DamageArea->SetCollisionObjectType(ECC_WorldDynamic);
	DamageArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	DamageArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Pawn만 감지
	
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

void ARushBossClone::MulticastPlayExplodeMontage()
{
	if (ExplodeMontage)
	{
		PlayAnimMontage(ExplodeMontage);
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
		if (IsValid(Cast<ADestructibleObject>(OtherActor)) && !OtherActor->IsHidden())
		{
			IBossPoolableActorInterface::Execute_OnPooledObjectReset(Cast<ADestructibleObject>(DestructibleObject));	
		}
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

		UE_LOG(LogTemp, Warning, TEXT("Arrived"));
		StartAttack();
	}
}

void ARushBossClone::StartAttack()
{
	if (!HasAuthority()) return;
	UE_LOG(LogTemp, Warning, TEXT("StartAttack"));
	//애니메이션
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(ExplodeMontage) && IsValid(AnimInstance))
	{
		if (!AnimInstance->OnMontageEnded.IsAlreadyBound(this, &ARushBossClone::OnMontageEnded))
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &ARushBossClone::OnMontageEnded);	
		}
	}
	PlayExplodeMontage();
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

void ARushBossClone::PlayExplodeMontage()
{
	if (HasAuthority())
	{
		MulticastPlayExplodeMontage();
	}
}

void ARushBossClone::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!HasAuthority()) return;
	if (Montage != ExplodeMontage) return;

	UAnimInstance* BossAnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(BossAnimInstance))
	{
		BossAnimInstance->OnMontageEnded.RemoveDynamic(this, &ARushBossClone::OnMontageEnded);
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	QueryParams.AddIgnoredActor(this); // 자기 자신 제외
	ABossCharacter* BossCharacter = Cast<ABossCharacter>(
		UGameplayStatics::GetActorOfClass(this, ABossCharacter::StaticClass()));
	QueryParams.AddIgnoredActor(BossCharacter);

	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		DamageArea->GetComponentLocation(),
		FQuat::Identity,
		ECC_Pawn, // Pawn 채널
		FCollisionShape::MakeSphere(DamageArea->GetScaledSphereRadius()), // Sphere
		QueryParams
	);

	if (bHit)
	{
		for (const FOverlapResult& Result : OverlapResults)
		{
			AActor* HitActor = Result.GetActor();
			if (IsValid(HitActor) && HitActor->IsA(ABaseWitch::StaticClass()))
			{
				ABaseWitch* HitWitch = Cast<ABaseWitch>(HitActor);
				FDamageEvent DamageEvent;
				HitWitch->TakeDamage(
					Damage,
					DamageEvent,
					GetInstigatorController(),
					this);
			}
		}
	}
	IBossPoolableActorInterface::Execute_OnPooledObjectReset(this);
}