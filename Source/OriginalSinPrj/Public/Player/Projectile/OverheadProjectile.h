// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Projectile/BaseProjectile.h"
#include "OverheadProjectile.generated.h"

UCLASS()
class ORIGINALSINPRJ_API AOverheadProjectile : public ABaseProjectile
{
	GENERATED_BODY()
	
public:	
	AOverheadProjectile();
	virtual void ActiveProjectile(const FAbilityDataBuffer& Buffer, float DelayTime) override;
	virtual void DeactiveProjectile() override;

protected:
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ActionDelay = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float MoveSpeed = 200.0f;

protected:
	FVector TargetDirection = FVector::ZeroVector;
	FTimerHandle ActionDelayTimer;
};
