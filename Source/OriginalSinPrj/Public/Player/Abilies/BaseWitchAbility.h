// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/WitchTypes.h"
#include "BaseWitchAbility.generated.h"

class ABaseWitch;
struct FAbilityDataBuffer;

UCLASS()
class ORIGINALSINPRJ_API ABaseWitchAbility : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWitchAbility();

	virtual void InitAbility() {};
	virtual bool ExcuteAbility(FAbilityDataBuffer& Buffer) { return true; };
	virtual void UndoAbility(FAbilityDataBuffer& Buffer) {};

	const EAbilityType& GetAbilityType() const;

protected:
	virtual void BeginPlay() override;
	virtual bool CheckExcuteable(FAbilityDataBuffer& Buffer) { return true; };

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TObjectPtr<UAnimMontage> AbilityMontage = nullptr;

protected:
	EAbilityType AbilityType = EAbilityType::None;
};
