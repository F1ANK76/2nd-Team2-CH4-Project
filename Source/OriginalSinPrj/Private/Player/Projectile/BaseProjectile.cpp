// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Projectile/BaseProjectile.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Player/Struct/AbilityDataBuffer.h"
#include "Player/BaseWitch.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Comp"));
	EffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect Comp"));

	SetRootComponent(SceneComp);
	CollisionComp->SetupAttachment(RootComponent);
	EffectComp->SetupAttachment(RootComponent);

	CollisionComp->SetGenerateOverlapEvents(true);
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ABaseProjectile::InitProjectile(ABaseWitch* Parent)
{
	ParentWitch = Parent;
}

void ABaseProjectile::ActiveProjectile(const FAbilityDataBuffer& Buffer, float DelayTime)
{
	if (DelayTime == 0)
	{
		SetActorEnableCollision(true);
		CollisionComp->Activate(true);
		OnActivedProjectile();
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, FTimerDelegate::CreateLambda([&]()
		{
			SetActorEnableCollision(true);
			CollisionComp->Activate(true);
			OnActivedProjectile();
		}), DelayTime, false);
}

void ABaseProjectile::DeactiveProjectile()
{
	if (bActiveOverlapEvent)
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(DelayTimer);
	CollisionComp->Deactivate();
	SetActorEnableCollision(false);
	OnDeactivedProjectile();
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	DeactiveProjectile();
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
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

	bActiveOverlapEvent = true;
	ParentWitch->ApplyAttack(OtherActor, DefaultDamage);
	bActiveOverlapEvent = false;
	DeactiveProjectile();
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

