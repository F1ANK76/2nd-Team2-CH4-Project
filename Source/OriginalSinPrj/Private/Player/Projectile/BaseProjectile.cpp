// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Projectile/BaseProjectile.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Player/Struct/AbilityDataBuffer.h"
#include "Player/BaseWitch.h"
#include "Player/Struct/ProjectileDataBuffer.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Comp"));
	EffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect Comp"));
	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Move Comp"));

	SetRootComponent(SceneComp);
	CollisionComp->SetupAttachment(RootComponent);
	EffectComp->SetupAttachment(RootComponent);
	
	CollisionComp->SetGenerateOverlapEvents(true);
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	MoveComp->bAutoActivate = false;

	CollisionComp->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
}

void ABaseProjectile::ActiveProjectile(const FProjectileDataBuffer& Buffer)
{
	ResetProjectile();
	bIsActivated = true;

	ParentWitch = Buffer.ParentWitch;
	AttackDamage = Buffer.AttackDamage;
	MoveDirection = Buffer.MoveDirection;
	MoveSpeed = Buffer.MoveSpeed;

	ActiveVisibleDelayTimer(Buffer.VisibleDelay);
	ActiveDeactiveDelayTimer(Buffer.DeactiveDelay);
	ActiveMoveDelayTimer(Buffer.VisibleDelay + Buffer.MoveDelay);
	ActiveAttackDelayTimer(Buffer.VisibleDelay + Buffer.MoveDelay + Buffer.AttackDelay);
}

void ABaseProjectile::DeactiveProjectile()
{
	if (!bCompleteDeactive)
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(DeactiveTimer);
	GetWorld()->GetTimerManager().ClearTimer(VisibleDelayTimer);
	GetWorld()->GetTimerManager().ClearTimer(AttackDelayTimer);
	GetWorld()->GetTimerManager().ClearTimer(MoveDelayTimer);

	CollisionComp->Deactivate();
	SetActorEnableCollision(false);
	bActiveOverlapEvent = false;

	MoveComp->Deactivate();
	MoveComp->InitialSpeed = 0;
	MoveComp->Velocity = FVector::ZeroVector;

	OnDeactivedProjectile();
	bIsActivated = false;
}

void ABaseProjectile::ApplyMove()
{
	if (MoveDirection.Size() == 0)
	{
		return;
	}

	if (IsValid(MoveComp))
	{
		MoveComp->Activate(true);
		MoveComp->InitialSpeed = MoveSpeed;
		MoveComp->Velocity = MoveDirection * MoveSpeed;
	}
}

void ABaseProjectile::ApplyAttack()
{
	SetActorEnableCollision(true);
	CollisionComp->Activate(true);
}

void ABaseProjectile::ApplyVisible()
{
	OnActivedProjectile();
}

EProjectileType ABaseProjectile::GetProjectileType() const
{
	return ProjectileType;
}

bool ABaseProjectile::GetIsActevated() const
{
	return bIsActivated;
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	ResetProjectile();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndedOverlap);
}

void ABaseProjectile::ActiveMoveDelayTimer(float TimeValue)
{
	if (TimeValue == 0)
	{
		ApplyMove();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(MoveDelayTimer, this, &ThisClass::ApplyMove, TimeValue, false);
	}
}

void ABaseProjectile::ActiveAttackDelayTimer(float TimeValue)
{
	if (TimeValue == 0)
	{
		ApplyAttack();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(AttackDelayTimer, this, &ThisClass::ApplyAttack, TimeValue, false);
	}
}

void ABaseProjectile::ActiveVisibleDelayTimer(float TimeValue)
{
	if (TimeValue == 0)
	{
		ApplyVisible();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(VisibleDelayTimer, this, &ThisClass::ApplyVisible, TimeValue, false);
	}
}

void ABaseProjectile::ActiveDeactiveDelayTimer(float TimeValue)
{
	if (TimeValue == 0)
	{
		bCompleteDeactive = true;
	}
	else
	{
		bCompleteDeactive = false;
		GetWorld()->GetTimerManager().SetTimer(DeactiveTimer, FTimerDelegate::CreateLambda([&]()
			{
				bCompleteDeactive = true;
				DeactiveProjectile();
			}), TimeValue, false);
	}
}

void ABaseProjectile::ResetProjectile()
{
	bCompleteDeactive = true;
	DeactiveProjectile();
}

void ABaseProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bActiveOverlapEvent)
	{
		return;
	}

	if (!IsValid(OtherActor))
	{
		return;
	}

	if (OtherActor == this)
	{
		return;
	}

	if (!IsValid(ParentWitch))
	{
		return;
	}

	if (OtherActor == ParentWitch)
	{
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("%s : Overlapped %s"), *GetName(), *OtherActor->GetName());

	bActiveOverlapEvent = true;
	ParentWitch->ApplyAttack(OtherActor, AttackDamage);

	if (bIsNotDelayDeactiveFromOverlap)
	{
		GetWorld()->GetTimerManager().ClearTimer(DeactiveTimer);
		bCompleteDeactive = true;
		DeactiveProjectile();
	}
}

void ABaseProjectile::OnEndedOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIsAgainAttack)
	{
		bActiveOverlapEvent = false;
	}
}

void ABaseProjectile::OnActivedProjectile_Implementation()
{
	if (IsValid(EffectComp))
	{
		EffectComp->Activate(true);
	}
}

void ABaseProjectile::OnDeactivedProjectile_Implementation()
{
	if (IsValid(EffectComp))
	{
		EffectComp->Deactivate();
	}
}

