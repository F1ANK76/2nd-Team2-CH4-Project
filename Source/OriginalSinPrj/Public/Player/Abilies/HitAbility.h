// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilies/BaseWitchAbility.h"
#include "HitAbility.generated.h"


UCLASS()
class ORIGINALSINPRJ_API AHitAbility : public ABaseWitchAbility
{
	GENERATED_BODY()
	
public:
	AHitAbility();
	virtual void InitAbility() override;
	virtual bool ExcuteAbility(FAbilityDataBuffer& Buffer) override;
	virtual void UndoAbility(FAbilityDataBuffer& Buffer) override;

protected:
	UFUNCTION(NetMulticast, Reliable)
	void ResponseOnLaunched(const FVector& KnockbackDistance);

	virtual bool CheckExcuteable(FAbilityDataBuffer& Buffer) override;
	virtual void Tick(float DeltaTime) override;

	void CalculateHitDirection(const FVector& Compare, const FVector& Target);
	void CalculateKnockTargetPos(FAbilityDataBuffer& Buffer);
	void CalculateStartedFalling();
	void OnEndedKnock();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
	float KnockDistanceY = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
	float KnockDistanceZ = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
	float KnockSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
	float KnockMultiply = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
	float KnockTime = 1.0f;

protected:
	UPROPERTY()
	TObjectPtr<ABaseWitch> Parent = nullptr;

	EDirectionType HitDirection = EDirectionType::None;
	FVector KnockDistance = FVector::ZeroVector;

	FString PreMoveState = "";
	bool bIsStartedKnock = false;
	bool bIsFalling = false;
	float StartPosZ = 0.0f;
};
