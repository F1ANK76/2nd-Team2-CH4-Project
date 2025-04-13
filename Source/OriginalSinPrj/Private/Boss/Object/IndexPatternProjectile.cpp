// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Object/IndexPatternProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AIndexPatternProjectile::AIndexPatternProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	Damage = 50.0f;
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

	if (!HasAuthority()) return;

	bIsActivate = false;
	MulticastSetActive(bIsActivate);
	ProjectileMovementComponent->StopMovementImmediately();
	ProjectileMovementComponent->Deactivate();
}

void AIndexPatternProjectile::OnPooledObjectSpawn_Implementation()
{
	if (!HasAuthority()) return;

	bIsActivate = true;
	MulticastSetActive(bIsActivate);
	ProjectileMovementComponent->Activate();

	GetWorldTimerManager().SetTimer(
		LifeTimeTimerHandle,
		this,
		&AIndexPatternProjectile::OnPooledObjectReset_Implementation,
		LifeTime,
		false);
}

void AIndexPatternProjectile::OnPooledObjectReset_Implementation()
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

void AIndexPatternProjectile::MulticastSetActive_Implementation(bool bIsActive)
{
	SetActorHiddenInGame(!bIsActive);
	SetActorEnableCollision(bIsActive);
}

void AIndexPatternProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweap, const FHitResult& SweapResult)
{
}

void AIndexPatternProjectile::SetDirection(const FVector& InDirection)
{
	Direction = InDirection;
}