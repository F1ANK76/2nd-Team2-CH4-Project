// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Object/IndexPatternProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "Net/UnrealNetwork.h"
#include "Player/BaseWitch.h"

AIndexPatternProjectile::AIndexPatternProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	Damage = 5.0f;
	LifeTime = 5.0f;
	Speed = 2500.0f;
	bIsActivate = false;

	bReplicates = true;
	SetReplicateMovement(true);

	//루트
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	//스태틱 메쉬
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	MeshComponent->SetupAttachment(SceneRoot);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//충돌 처리 Sphere Collision
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(SceneRoot);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AIndexPatternProjectile::OnOverlapBegin);

	//Niagara
	NiagaraComponent1 = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent1");
	NiagaraComponent1->SetupAttachment(SceneRoot);
	NiagaraComponent1->SetAutoActivate(false);
	NiagaraComponent2 = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent2");
	NiagaraComponent2->SetupAttachment(SceneRoot);
	NiagaraComponent2->SetAutoActivate(false);

	//투사체 - ProjectileMovementComponent
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->InitialSpeed = Speed;
	ProjectileMovementComponent->MaxSpeed = Speed;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f; //중력 영향 없음
}

void AIndexPatternProjectile::BeginPlay()
{
	Super::BeginPlay();

	bIsActivate = false;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	ProjectileMovementComponent->StopMovementImmediately();
	ProjectileMovementComponent->Deactivate();
}

void AIndexPatternProjectile::OnPooledObjectSpawn_Implementation()
{
	bIsActivate = true;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	ProjectileMovementComponent->Activate();

	ApplyMaterialFromIndex(bIsFirstIndex);
	ApplyNiagaraFromIndex(bIsFirstIndex);

	NiagaraToActive->SetActive(true);
	
	GetWorldTimerManager().SetTimer(
		LifeTimeTimerHandle,
		this,
		&AIndexPatternProjectile::OnPooledObjectReset_Implementation,
		LifeTime,
		false);
}

void AIndexPatternProjectile::OnPooledObjectReset_Implementation()
{
	bIsActivate = false;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	ProjectileMovementComponent->StopMovementImmediately();
	ProjectileMovementComponent->Deactivate();
	ProjectileMovementComponent->Velocity = FVector::ZeroVector;
	NiagaraToActive->SetActive(false);
	
	if (GetWorldTimerManager().IsTimerActive(LifeTimeTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(LifeTimeTimerHandle);
	}
}

void AIndexPatternProjectile::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AIndexPatternProjectile, bIsFirstIndex);
}

void AIndexPatternProjectile::MulticastSetActive_Implementation(bool bIsActive)
{
	SetActorHiddenInGame(!bIsActive);
	SetActorEnableCollision(bIsActive);
}

void AIndexPatternProjectile::OnRep_IsFirstIndex()
{
	ApplyMaterialFromIndex(bIsFirstIndex);
	ApplyNiagaraFromIndex(bIsFirstIndex);
}

void AIndexPatternProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweap,
                                             const FHitResult& SweapResult)
{
	if (!HasAuthority()) return;
	
	if (IsValid(OtherActor) && OtherActor->IsA(ABaseWitch::StaticClass()))
	{
		ABaseWitch* HitWitch = Cast<ABaseWitch>(OtherActor);
		if (IsValid(HitWitch))
		{
			FDamageEvent DamageEvent;

			if (HitWitch->GetColorIndex() == bIsFirstIndex)
			{
				HitWitch->TakeDamage(
				Damage,
				DamageEvent,
				GetInstigatorController(),
				this);
			}
			
			IBossPoolableActorInterface::Execute_OnPooledObjectReset(this);
		}
	}
	
	if (IsValid(OtherActor) && OtherActor->ActorHasTag("Ground"))
	{
		IBossPoolableActorInterface::Execute_OnPooledObjectReset(this);
	}
}

void AIndexPatternProjectile::ApplyMaterialFromIndex(bool bFirst)
{
	UMaterialInterface* MaterialToApply = bFirst ? Material1 : Material2;
	if (IsValid(MaterialToApply))
	{
		MeshComponent->SetMaterial(0, MaterialToApply);
	}
}

void AIndexPatternProjectile::ApplyNiagaraFromIndex(bool bFirst)
{
	if (IsValid(NiagaraComponent1) && IsValid(NiagaraComponent2))
	{
		NiagaraToActive = bFirst ? NiagaraComponent1 : NiagaraComponent2;
	}
}


void AIndexPatternProjectile::SetDirectionAndVelocity(const FVector& InDirection)
{
	Direction = InDirection;
	ProjectileMovementComponent->Velocity = Direction * Speed;
}

void AIndexPatternProjectile::SetIndex(int32 InIndex)
{
	if (!HasAuthority()) return;
	
	UMaterialInterface* MaterialToApply = nullptr;

	//InIndex : 0, 1, 2, 3
	//bIsFirstIndex : true, false, true, false
	bIsFirstIndex = ((InIndex + 1) % 2 == 1);
	ApplyMaterialFromIndex(bIsFirstIndex);
	ApplyNiagaraFromIndex(bIsFirstIndex);
}
