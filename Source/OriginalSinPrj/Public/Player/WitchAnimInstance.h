// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Player/WitchTypes.h"
#include "WitchAnimInstance.generated.h"

class UCharacterMovementComponent;
class ABaseWitch;

UCLASS()
class ORIGINALSINPRJ_API UWitchAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite)
	EWitchStateType CharacterState = EWitchStateType::Idle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ECharacterType CharacterType = ECharacterType::Hand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Speed = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsInAir = false;

protected:
	TObjectPtr<UCharacterMovementComponent> MovementComp = nullptr;
	TObjectPtr<ABaseWitch> OwnCharacter = nullptr;
};
