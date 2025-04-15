// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Object/BossPlatform.h"

ABossPlatform::ABossPlatform()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	MeshComponent->SetupAttachment(SceneRoot);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionProfileName("BlockAll");
	MeshComponent->OnComponentHit.AddDynamic(this, &ABossPlatform::OnPlatformHit);

	bReplicates = true;
	SetReplicateMovement(true);

	Tags.Add("BossPlatform");
	Tags.Add("PatternPlatform");
}

void ABossPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()) return;

	bIsActivate = false;
	SetActorHiddenInGame(true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABossPlatform::OnPooledObjectSpawn_Implementation()
{
	bIsActivate = true;
	SetActorHiddenInGame(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ABossPlatform::OnPooledObjectReset_Implementation()
{
	bHasBeenTriggered = false;
	bIsActivate = false;
	SetActorHiddenInGame(true);
	SetActorLocation(FVector::ZeroVector);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABossPlatform::OnPlatformHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HasAuthority()) return;
	if (bHasBeenTriggered) return;

	if (IsValid(OtherActor) && OtherActor->ActorHasTag("Player"))
	{
		bHasBeenTriggered = true;
		StartDisappearTimer();
	}
}

void ABossPlatform::StartDisappearTimer()
{
	GetWorldTimerManager().SetTimer(
		DisappearTimerHandle,
		this,
		&ABossPlatform::DisappearPlatform,
		DisappearDelay,
		false);
}

void ABossPlatform::DisappearPlatform()
{
	IBossPoolableActorInterface::Execute_OnPooledObjectReset(this);
}
