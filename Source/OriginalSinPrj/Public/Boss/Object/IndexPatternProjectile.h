// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossPoolableActorInterface.h"
#include "NiagaraComponent.h"
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

	void SetDirectionAndVelocity(const FVector& InDirection);
	void SetIndex(int32 InIndex);

protected:
	virtual void BeginPlay() override;
	virtual void OnPooledObjectSpawn_Implementation() override;
	virtual void OnPooledObjectReset_Implementation() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, Category = "IndexPatternProjectile | Components")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "IndexPatternProjectile | Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "IndexPatternProjectile | Components")
	USphereComponent* SphereComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "IndexPatternProjectile | Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IndexPatternProjectile | Components")
	UNiagaraComponent* NiagaraComponent1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IndexPatternProjectile | Components")
	UNiagaraComponent* NiagaraComponent2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IndexPatternProjectile | Property")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IndexPatternProjectile | Property")
	float LifeTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IndexPatternProjectile | Property")
	float Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IndexPatternProjectile | Material")
	UMaterialInterface* Material1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IndexPatternProjectile | Material")
	UMaterialInterface* Material2;
	
	
	UPROPERTY(ReplicatedUsing = OnRep_IsFirstIndex)
	bool bIsFirstIndex = false;

	FTimerHandle LifeTimeTimerHandle;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetActive(bool bIsActive);

	UFUNCTION()
	void OnRep_IsFirstIndex();
	
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweap,
		const FHitResult& SweapResult);

private:
	UPROPERTY()
	UNiagaraComponent* NiagaraToActive;
	
	FVector Direction;
	bool bIsActivate;

	void ApplyMaterialFromIndex(bool bFirst);
	void ApplyNiagaraFromIndex(bool bFirst);
};
