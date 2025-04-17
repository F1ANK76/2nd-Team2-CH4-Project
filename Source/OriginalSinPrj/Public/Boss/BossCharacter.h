// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossCharacter.generated.h"

class UBTTaskNode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossTakeDamage, float, NewHp);

UCLASS()
class ORIGINALSINPRJ_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABossCharacter();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property")
	int32 MaxHP;

	UPROPERTY(BlueprintAssignable)
	FOnBossTakeDamage OnBossTakeDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* StartBattleMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* StartStunMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* EndStunMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* RangeAttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* AreaSpawnWeaponMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* RushBossAttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* HijackAttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* InstantDeathAttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* IndexAttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* KillAllPlayerAttackMontage;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayStartBattleMontage();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayDeathMontage();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayStartStunMontage();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayEndStunMontage();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayRangeAttackMontage();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayAreaSpawnWeaponMontage();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayRushBossAttackMontage();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayHijackAttackMontage();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayInstantDeathAttackMontage();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayIndexAttackMontage();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayKillAllPlayerAttackMontage();

	void PlayStartBattleMontage();
	void PlayDeathMontage();
	void PlayStartStunMontage();
	void PlayEndStunMontage();
	void PlayRangeAttackMontage();
	void PlayAreaSpawnWeaponMontage();
	void PlayRushBossAttackMontage();
	void PlayHijackAttackMontage();
	void PlayInstantDeathAttackMontage();
	void PlayIndexAttackMontage();
	void PlayKillAllPlayerAttackMontage();

	void UpdateFacingDirection(APawn* ClosestPlayer);

	//Getter
	bool GetIsDead() const { return bIsDead; }
	int32 GetCurrentHP() const { return CurrentHP; }
	int32 GetMaxHP() const { return MaxHP; }
	UBTTaskNode* GetCurrentTaskNode() const{ return CurrentTaskNode; }
	bool GetIsInvincibility() const { return bIsInvincibility; }
	//Setter
	void SetCurrentTaskNode(UBTTaskNode* NewTaskNode) { CurrentTaskNode = NewTaskNode; }
	void SetIsInvincibility(bool NewBool) { bIsInvincibility = NewBool; }
	
protected:
	UPROPERTY()
	UBTTaskNode* CurrentTaskNode = nullptr;
	
private:
	int32 CurrentHP;
	bool bIsDead;
	bool bIsInvincibility;

	void SetFacingDirection(float Direction);
};
