// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Object/RangeAttackProjectile.h"

#include "NiagaraComponent.h"
#include "Engine/DamageEvents.h"
#include "Boss/BossCharacter.h"
#include "Boss/BossObjectPoolWorldSubsystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/DamageType.h"
#include "Components/SphereComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BaseWitch.h"

ARangeAttackProjectile::ARangeAttackProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	Damage = 10.0f;
	LifeTime = 3.0f;
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
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ARangeAttackProjectile::OnOverlapBegin);

	//Niagara
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	NiagaraComponent->SetupAttachment(SceneRoot);
	NiagaraComponent->SetAutoActivate(false);

	//투사체 - ProjectileMovementComponent
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->InitialSpeed = 4000.0f;
	ProjectileMovementComponent->MaxSpeed = 4000.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f; //중력 영향 없음
}

void ARangeAttackProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()) return;
	
	bIsActivate = false;
	MulticastSetActive(bIsActivate);
	ProjectileMovementComponent->StopMovementImmediately();
	ProjectileMovementComponent->Deactivate();
}

void ARangeAttackProjectile::OnPooledObjectSpawn_Implementation()
{
	if (!HasAuthority()) return;

	bIsActivate = true;
	MulticastSetActive(bIsActivate);
	ProjectileMovementComponent->Activate();

	GetWorldTimerManager().SetTimer(
		LifeTimeTimerHandle,
		this,
		&ARangeAttackProjectile::OnPooledObjectReset_Implementation,
		LifeTime,
		false);
}

void ARangeAttackProjectile::OnPooledObjectReset_Implementation()
{
	if (!HasAuthority()) return;

	bIsActivate = false;
	MulticastSetActive(bIsActivate);
	ProjectileMovementComponent->StopMovementImmediately();
	ProjectileMovementComponent->Deactivate();
	ProjectileMovementComponent->Velocity = FVector::ZeroVector;

	if (GetWorldTimerManager().IsTimerActive(LifeTimeTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(LifeTimeTimerHandle);
	}
}

void ARangeAttackProjectile::MulticastSetActive_Implementation(bool bIsActive)
{
	SetActorHiddenInGame(!bIsActive);
	SetActorEnableCollision(bIsActive);
	if (IsValid(NiagaraComponent))
	{
		NiagaraComponent->SetActive(bIsActive);
	}
}

void ARangeAttackProjectile::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex,
	bool bFromSweap,
	const FHitResult& SweapResult)
{
	if (IsValid(OtherActor) && OtherActor != this && IsValid(OtherComponent) && HasAuthority())
	{
		//보스 자신 제외
		if (OtherActor->Tags.Contains("Boss")) return;

		if (IsValid(OtherActor) && OtherActor->IsA(ABaseWitch::StaticClass()))
		{
			ABaseWitch* HitWitch = Cast<ABaseWitch>(OtherActor);
			if (IsValid(HitWitch))
			{
				FDamageEvent DamageEvent;

				HitWitch->TakeDamage(
					Damage,
					DamageEvent,
					GetInstigatorController(),
					this);
			}
		}

		//땅에 닿았을 때
		if (OtherActor->Tags.Contains("Ground"))
		{
			IBossPoolableActorInterface::Execute_OnPooledObjectReset(this);
		}
	}
}

void ARangeAttackProjectile::SetTargetDirection(const FVector& TargetLocation)
{
	if (!IsValid(ProjectileMovementComponent)) return;

	if (HasAuthority())
	{
		//타겟 방향의 단위벡터 계산(방향 벡터)
		TargetDirection = (TargetLocation - GetActorLocation()).GetSafeNormal();

		//속도 = 방향 벡터 * 속력설정값
		ProjectileMovementComponent->Velocity = TargetDirection * ProjectileMovementComponent->InitialSpeed;
	}
}
