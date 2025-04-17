// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HijackBossCharacter.generated.h"

class ABossCharacter;
class ABossController;

UCLASS()
class ORIGINALSINPRJ_API AHijackBossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AHijackBossCharacter();

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* CastingMontage;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayCastingMontage();

	void PlayCastingMontage();
	
	void UpdateFacingDirection(APawn* ClosestPlayer);

	int32 GetCurrentHP() const { return CurrentHP; }
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ABossCharacter* BossCharacter;
	UPROPERTY()
	ABossController* BossController;
	
	int32 CurrentHP;
	bool bIsDead;

	void SetFacingDirection(float Direction);
};
