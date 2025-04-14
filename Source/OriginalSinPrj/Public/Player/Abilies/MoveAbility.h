// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilies/BaseWitchAbility.h"
#include "MoveAbility.generated.h"


UCLASS()
class ORIGINALSINPRJ_API AMoveAbility : public ABaseWitchAbility
{
	GENERATED_BODY()
	
public:
	virtual void InitAbility() override;
	virtual bool ExcuteAbility(FAbilityDataBuffer& Buffer) override;
	
protected:
	virtual bool CheckExcuteable(FAbilityDataBuffer& Buffer) override;
	void ExcuteMove(FAbilityDataBuffer& Buffer);
	void ExcuteComand(FAbilityDataBuffer& Buffer);

	UFUNCTION(NetMulticast, Reliable)
	void ResponseRotated(APawn* ParentWitch, bool bIsRotate);

private:
	static const float RotateValue;
};
