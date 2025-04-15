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
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* HijackAttackMontage;

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
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayHijackAttackMontage();

	void PlayStartBattleMontage();
	void PlayDeathMontage();
	void PlayRangeAttackMontage();
	void PlayAreaSpawnWeaponMontage();
	void PlayRushBossAttackMontage();
	void PlayHijackAttackMontage();

	void UpdateFacingDirection(APawn* ClosestPlayer);

	bool GetIsDead() const { return bIsDead; }
	int32 GetCurrentHP() const { return CurrentHP; }
	int32 GetMaxHP() const { return MaxHP; }
	
private:
	int32 CurrentHP;
	bool bIsDead;

	void SetFacingDirection(float Direction);
};
