// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Object/DestructibleObject.h"

#include "Boss/BossController.h"
#include "Engine/DamageEvents.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BaseWitch.h"


ADestructibleObject::ADestructibleObject()
{
	PrimaryActorTick.bCanEverTick = false;

	CurrentHp = 1;
	bIsActivate = false;
	
	bReplicates = true;
	SetReplicateMovement(true);

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

float ADestructibleObject::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (!HasAuthority()) return 0.0f;
	if (!IsValid(DamageCauser)) return 0.0f;
	
	CurrentHp -= DamageAmount;
	if (CurrentHp < 0) CurrentHp = 0;

	if (CurrentHp <= 0) IBossPoolableActorInterface::Execute_OnPooledObjectReset(this);
	
	return 0.0f;
}

void ADestructibleObject::OnPooledObjectSpawn_Implementation()
{
	if (!HasAuthority()) return;
	
	BossController = Cast<ABossController>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossController::StaticClass()));
	BossController->SetOnePlusDestructibleObjectCount();
	bIsActivate = true;
	MulticastSetActive(bIsActivate);
}

void ADestructibleObject::OnPooledObjectReset_Implementation()
{
	if (!HasAuthority()) return;

	BossController->SetOneMinusDestructibleObjectCount();
	bIsActivate = false;
	MulticastSetActive(bIsActivate);
}

void ADestructibleObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	//파괴
	if (CurrentHp <= 0)
	{
		//파괴 이펙트 추가 필요 : OnPooledObjectReset쪽에 붙이는것도 괜찮을지도?

		IBossPoolableActorInterface::Execute_OnPooledObjectReset(this);
	}
}

void ADestructibleObject::MulticastSetActive_Implementation(bool bIsActive)
{
	SetActorHiddenInGame(!bIsActive);
}
