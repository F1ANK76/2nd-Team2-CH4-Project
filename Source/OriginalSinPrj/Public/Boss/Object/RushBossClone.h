// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossPoolableActorInterface.h"
#include "GameFramework/Character.h"
#include "RushBossClone.generated.h"

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
	UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RushBossClone | Property")
	float ZOffset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RushBossClone | Property")
	float AcceptableDistance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RushBossClone | Property")
	float RushSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RushBossClone | Property")
	float AttackDuration;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* CloneAttackMontage;

	UFUNCTION()
	void MulticastPlayCloneAttackMontage();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetActive(bool bIsActive);

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
	bool bIsRushing;
	bool bHasArrived;
	bool bIsActivate;
	FTimerHandle DisappearTimerHandle;
};