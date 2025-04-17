// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/WitchTypes.h"
#include "BaseWitchAbility.generated.h"

class ABaseWitch;
class ABaseGameState;
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

	bool CheckValidOfGameState();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Settings")
	TObjectPtr<UAnimMontage> AbilityMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Settings")
	EAbilityType AbilityType = EAbilityType::None;

protected:
	UPROPERTY()
	TObjectPtr<ABaseGameState> BaseGameState = nullptr;
};
