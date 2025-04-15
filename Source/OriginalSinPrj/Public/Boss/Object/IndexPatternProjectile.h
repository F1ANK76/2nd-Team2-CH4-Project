// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossPoolableActorInterface.h"
#include "GameFramework/Actor.h"
#include "IndexPatternProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class ORIGINALSINPRJ_API AIndexPatternProjectile : public AActor, public IBossPoolableActorInterface
{
	GENERATED_BODY()

public:
	AIndexPatternProjectile();

	void SetDirection(const FVector& InDirection);

protected:
	virtual void BeginPlay() override;
	virtual void OnPooledObjectSpawn_Implementation() override;
	virtual void OnPooledObjectReset_Implementation() override;

	UPROPERTY(VisibleAnywhere, Category = "IndexPatternProjectile | Components")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "IndexPatternProjectile | Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "IndexPatternProjectile | Components")
	USphereComponent* SphereComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "IndexPatternProjectile | Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IndexPatternProjectile | Property")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IndexPatternProjectile | Property")
	float LifeTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IndexPatternProjectile | Property")
	float Speed;

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
	FVector Direction;
	bool bIsActivate;
};
