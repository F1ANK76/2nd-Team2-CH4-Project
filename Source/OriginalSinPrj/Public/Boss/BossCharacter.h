// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossCharacter.generated.h"

UCLASS()
class ORIGINALSINPRJ_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABossCharacter();
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* RangeAttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* AreaSpawnWeaponMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* RushBossAttackMontage;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayRangeAttackMontage();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayAreaSpawnWeaponMontage();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayRushBossAttackMontage();
	
	void PlayRangeAttackMontage();
	void PlayAreaSpawnWeaponMontage();
	void PlayRushBossAttackMontage();
	
	void UpdateFacingDirection(APawn* ClosestPlayer);

private:
	void SetFacingDirection(float Direction);
};