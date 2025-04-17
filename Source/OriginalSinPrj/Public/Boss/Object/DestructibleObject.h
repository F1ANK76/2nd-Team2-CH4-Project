// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossPoolableActorInterface.h"
#include "GameFramework/Actor.h"
#include "DestructibleObject.generated.h"

class ABossController;
class USphereComponent;
class UNiagaraSystem;

UCLASS()
class ORIGINALSINPRJ_API ADestructibleObject : public AActor, public IBossPoolableActorInterface
{
	GENERATED_BODY()

public:
	ADestructibleObject();
	
	void SetBossControllerCache(ABossController* InBossController) { BossController = InBossController; }

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount,struct FDamageEvent const& DamageEvent,class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnPooledObjectSpawn_Implementation() override;
	virtual void OnPooledObjectReset_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USphereComponent* SphereComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponToSpawn | Components")
	UAudioComponent* AudioComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	int32 CurrentHp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* ExplosionEffect;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetActive(bool bIsActive);

private:
	UPROPERTY()
	ABossController* BossController;

	bool bIsActivate;
};
