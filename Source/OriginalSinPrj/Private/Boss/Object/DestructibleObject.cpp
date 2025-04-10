// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Object/DestructibleObject.h"

#include "Components/SphereComponent.h"


ADestructibleObject::ADestructibleObject()
{
	PrimaryActorTick.bCanEverTick = false;

	//3번 공격하면 파괴
	HP = 3;
	bIsActivate = false;
	
	bReplicates = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	MeshComponent->SetupAttachment(SceneRoot);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(MeshComponent);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADestructibleObject::OnOverlapBegin);
}

void ADestructibleObject::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()) return;
	
	bIsActivate = false;
	SetActorHiddenInGame(true);
}

void ADestructibleObject::OnPooledObjectSpawn_Implementation()
{
	if (!HasAuthority()) return;
	
	bIsActivate = true;
	MulticastSetActive(bIsActivate);
}

void ADestructibleObject::OnPooledObjectReset_Implementation()
{
	if (!HasAuthority()) return;
	
	bIsActivate = false;
	MulticastSetActive(bIsActivate);
}

float ADestructibleObject::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, int32 DamageTypeID,
	AController* EventInstigator, AActor* DamageCauser)
{
	if (!IsValid(DamageCauser)) return 0.0f;

	//플레이어 합친 이후 데미지 로직 작성 필요

	//임시
	return 0.0f;
}

void ADestructibleObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	//데미지 로직 수행

	//파괴
	if (HP <= 0)
	{
		//파괴 이펙트 로직 추가 필요

		IBossPoolableActorInterface::Execute_OnPooledObjectReset(this);
	}
}

void ADestructibleObject::MulticastSetActive_Implementation(bool bIsActive)
{
	SetActorHiddenInGame(!bIsActive);
}
