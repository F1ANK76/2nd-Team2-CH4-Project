// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossPoolableActorInterface.h"
#include "RangeAttackProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class ORIGINALSINPRJ_API ARangeAttackProjectile : public AActor, public IBossPoolableActorInterface
{
	GENERATED_BODY()
	
public:
	ARangeAttackProjectile();

	void SetTargetDirection(const FVector& TargetLocation);
	
protected:
	virtual void BeginPlay() override;
	virtual void OnPooledObjectSpawn_Implementation() override;
	virtual void OnPooledObjectReset_Implementation() override;

	UPROPERTY(VisibleAnywhere, Category = "RangeAttack | Components")
	USceneComponent* SceneRoot;
	
	UPROPERTY(VisibleAnywhere, Category = "RangeAttack | Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "RangeAttack | Components")
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, Category = "RangeAttack | Components")
	UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RangeAttack | Property")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RangeAttack | Property")
	float LifeTime;

	FTimerHandle LifeTimeTimerHandle;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetActive(bool bIsActive);
	
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweap,
		const FHitResult& SweapResult);
private:
	FVector TargetDirection;
	bool bIsActivate;
};
