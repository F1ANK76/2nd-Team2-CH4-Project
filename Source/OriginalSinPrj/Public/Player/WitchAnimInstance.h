// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Player/WitchTypes.h"
#include "WitchAnimInstance.generated.h"

class UCharacterMovementComponent;

UCLASS()
class ORIGINALSINPRJ_API UWitchAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	void SetAnimState(const EWitchStateType NewState);

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EWitchStateType AnimState = EWitchStateType::Idle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Speed = 0.0f;

protected:
	TObjectPtr<UCharacterMovementComponent> MovementComp = nullptr;

};
