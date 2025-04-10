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
	virtual bool CheckExcuteable(FAbilityDataBuffer& Buffer) override;
	virtual void Tick(float DeltaTime) override;

protected:
	bool bCompleteKnock = false;
	bool bIsHittedFromLeft = false;

	FVector HitDirection = FVector::ZeroVector;
};
