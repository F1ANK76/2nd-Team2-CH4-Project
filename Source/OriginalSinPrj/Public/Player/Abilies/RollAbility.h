// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilies/BaseWitchAbility.h"
#include "RollAbility.generated.h"


UCLASS()
class ORIGINALSINPRJ_API ARollAbility : public ABaseWitchAbility
{
	GENERATED_BODY()
	
public:
	virtual void InitAbility() override;
	virtual bool ExcuteAbility(FAbilityDataBuffer& Buffer) override;
	virtual void UndoAbility(FAbilityDataBuffer& Buffer) override;
	
protected:
	virtual bool CheckExcuteable(FAbilityDataBuffer& Buffer) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TObjectPtr<UAnimMontage> BackRoll = nullptr;
};
