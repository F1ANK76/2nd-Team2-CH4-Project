// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossPoolableActorInterface.h"
#include "GameFramework/Character.h"
#include "RushBossClone.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class ORIGINALSINPRJ_API ARushBossClone : public ACharacter, public IBossPoolableActorInterface
{
	GENERATED_BODY()

public:
	ARushBossClone();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPooledObjectSpawn_Implementation() override;
	virtual void OnPooledObjectReset_Implementation() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RushBossClone | Components")
	USphereComponent* SphereComponent = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RushBossClone | Property")
	float ZOffset = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RushBossClone | Property")
	float AcceptableDistance = 300.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RushBossClone | Property")
	float RushSpeed = 12000.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RushBossClone | Property")
	float AttackDuration = 2.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RushBossClone | Property")
	float LifeTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* CloneAttackMontage;

	UFUNCTION()
	void MulticastPlayCloneAttackMontage();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetActive(bool bIsActive);

	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	void InitializeClone(const FVector& InTargetLocation);
	void CheckArrival();
	void StartAttack();
	void FinishAttack();
	void SetFacingDirection();

	//애니메이션
	void PlayCloneAttackMontage();

private:
	FVector TargetLocation;
	FVector TargetDirection;
	bool bIsRushing = false;
	bool bHasArrived = false;
	bool bIsActivate;
	FTimerHandle LifeTimeTimerHandle;
	FTimerHandle DisappearTimerHandle;
};