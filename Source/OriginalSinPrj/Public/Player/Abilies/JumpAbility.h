// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilies/BaseWitchAbility.h"
#include "JumpAbility.generated.h"

class ABaseWitch;
class UCharacterMovementComponent;

UCLASS()
class ORIGINALSINPRJ_API AJumpAbility : public ABaseWitchAbility
{
	GENERATED_BODY()
	
public:
	AJumpAbility();
	virtual void InitAbility() override;
	virtual bool ExcuteAbility(FAbilityDataBuffer& Buffer) override;
	virtual void UndoAbility(FAbilityDataBuffer& Buffer) override;
	
protected:
	UFUNCTION(NetMulticast, Reliable)
	void ResponseJumped(ABaseWitch* ParentWitch);

	virtual bool CheckExcuteable(FAbilityDataBuffer& Buffer) override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY()
	TObjectPtr<ABaseWitch> Parent = nullptr;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MoveComp = nullptr;

	bool bIsJumping = false;
};
