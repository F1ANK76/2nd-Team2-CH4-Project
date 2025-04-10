// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Projectile/OverheadProjectile.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Player/Struct/AbilityDataBuffer.h"

AOverheadProjectile::AOverheadProjectile() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AOverheadProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector AddedLocation = GetActorLocation() + TargetDirection * MoveSpeed * DeltaTime;

	SetActorLocation(AddedLocation);
}

void AOverheadProjectile::ActiveProjectile(const FAbilityDataBuffer& Buffer, float DelayTime)
{
	Super::ActiveProjectile(Buffer, DelayTime);

	if (Buffer.bIsLeft)
	{
		TargetDirection.Y = -3;
		SetActorRotation(FRotator(0, -90, 0));
	}
	else
	{
		TargetDirection.Y = 3;
		SetActorRotation(FRotator(0, 90, 0));
	}
	
	TargetDirection.Z = -1.0f;

	if (ActionDelay == 0)
	{
		SetActorTickEnabled(true);
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(ActionDelayTimer, FTimerDelegate::CreateLambda([&]()
		{
			SetActorTickEnabled(true);
		}), DelayTime + ActionDelay, false);
}

void AOverheadProjectile::DeactiveProjectile()
{
	Super::DeactiveProjectile();

	GetWorld()->GetTimerManager().ClearTimer(ActionDelayTimer);
	SetActorTickEnabled(false);
}

