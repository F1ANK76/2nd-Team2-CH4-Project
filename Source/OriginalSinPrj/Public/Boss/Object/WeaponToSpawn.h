// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossPoolableActorInterface.h"
#include "GameFramework/Actor.h"
#include "WeaponToSpawn.generated.h"

class UProjectileMovementComponent;
class UCapsuleComponent;

UCLASS()
class ORIGINALSINPRJ_API AWeaponToSpawn : public AActor, public IBossPoolableActorInterface
{
	GENERATED_BODY()
	
public:
	AWeaponToSpawn();

	void SetTargetDirection(const FVector& TargetLocation);
	
protected:
	virtual void BeginPlay() override;
	virtual void OnPooledObjectSpawn_Implementation() override;
	virtual void OnPooledObjectReset_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponToSpawn | Components")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponToSpawn | Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponToSpawn | Components")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponToSpawn | Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponToSpawn | Components")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponToSpawn | Property")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponToSpawn | Property")
	float LifeTimeOnGround;

	FTimerHandle LifeTimeTimerHandle;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetActive(bool bIsActive);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastOffProjectileMovement();
	
	UFUNCTION()
	void OnMeshOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnCapsuleOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

private:
	FVector TargetDirection;
	bool bIsActivate;
};
