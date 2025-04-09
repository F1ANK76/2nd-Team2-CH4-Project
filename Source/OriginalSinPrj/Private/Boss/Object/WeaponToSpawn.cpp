// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Object/WeaponToSpawn.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"

AWeaponToSpawn::AWeaponToSpawn()
{
	PrimaryActorTick.bCanEverTick = false;

	Damage = 50.0f;
	LifeTimeOnGround = 2.0f;
	bIsActivate = false;

	bReplicates = true;
	SetReplicateMovement(true);

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	MeshComponent->SetupAttachment(SceneRoot);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeaponToSpawn::OnMeshOverlapBegin);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetupAttachment(SceneRoot);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeaponToSpawn::OnCapsuleOverlapBegin);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->InitialSpeed = 10000.0f;
	ProjectileMovementComponent->MaxSpeed = 10000.0f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f; //중력 영향 없음
}

void AWeaponToSpawn::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority()) return;

	bIsActivate = false;
	MulticastSetActive(bIsActivate);
}

void AWeaponToSpawn::OnPooledObjectSpawn_Implementation()
{
	if (!HasAuthority()) return;

	bIsActivate = true;
	MulticastSetActive(bIsActivate);
}

void AWeaponToSpawn::OnPooledObjectReset_Implementation()
{
	if (!HasAuthority()) return;

	bIsActivate = false;
	MulticastSetActive(bIsActivate);
}

void AWeaponToSpawn::MulticastSetActive_Implementation(bool bIsActive)
{
	SetActorHiddenInGame(!bIsActive);
	SetActorEnableCollision(bIsActive);

	if (bIsActive)
	{
		ProjectileMovementComponent->Activate();
	}
	else if (!bIsActive)
	{
		ProjectileMovementComponent->StopMovementImmediately();
		ProjectileMovementComponent->Deactivate();
		ProjectileMovementComponent->Velocity = FVector::ZeroVector;

		if (GetWorldTimerManager().IsTimerActive(LifeTimeTimerHandle))
		{
			GetWorldTimerManager().ClearTimer(LifeTimeTimerHandle);
		}
	}
}

void AWeaponToSpawn::MulticastOffProjectileMovement_Implementation()
{
	ProjectileMovementComponent->StopMovementImmediately();
	ProjectileMovementComponent->Deactivate();
}

//플레이어와 닿았을때를 위한 로직 : MeshComponent
void AWeaponToSpawn::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	//플레이어에 대한 추가적 로직 코드 필요함
	if (IsValid(OtherActor))
	{
		//데미지 주는 로직 작성 필요
	}
}

//레벨 구조물과 닿았을 때 정지하기 위한 로직 : CapsuleComponent
void AWeaponToSpawn::OnCapsuleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                           const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && OtherActor != this && IsValid(OtherComponent) && HasAuthority())
	{
		if (OtherActor->Tags.Contains("Ground"))
		{
			MulticastOffProjectileMovement();
			GetWorldTimerManager().SetTimer(
				LifeTimeTimerHandle,
				this,
				&AWeaponToSpawn::OnPooledObjectReset_Implementation,
				LifeTimeOnGround,
				false);
		}
	}
}

void AWeaponToSpawn::SetTargetDirection(const FVector& TargetLocation)
{
	if (!IsValid(ProjectileMovementComponent)) return;

	if (!HasAuthority()) return;

	//타겟 방향의 단위벡터 계산(방향 벡터)
	TargetDirection = (TargetLocation - GetActorLocation()).GetSafeNormal();

	//속도 = 방향 벡터 * 속력설정값
	ProjectileMovementComponent->Velocity = TargetDirection * ProjectileMovementComponent->InitialSpeed;
}
