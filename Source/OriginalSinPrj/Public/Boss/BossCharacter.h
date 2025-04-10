// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDeath);

UCLASS()
class ORIGINALSINPRJ_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABossCharacter();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	int32 MaxHP;

	UPROPERTY(BlueprintAssignable)
	FOnBossDeath OnBossDeath;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* StartBattleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* RangeAttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* AreaSpawnWeaponMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* RushBossAttackMontage;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayStartBattleMontage();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayDeathMontage();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayRangeAttackMontage();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayAreaSpawnWeaponMontage();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayRushBossAttackMontage();

	void PlayStartBattleMontage();
	void PlayDeathMontage();
	void PlayRangeAttackMontage();
	void PlayAreaSpawnWeaponMontage();
	void PlayRushBossAttackMontage();

	void UpdateFacingDirection(APawn* ClosestPlayer);

	bool GetIsDead() const { return bIsDead; }

private:
	int32 CurrentHP;
	bool bIsDead;

	void SetFacingDirection(float Direction);
};
