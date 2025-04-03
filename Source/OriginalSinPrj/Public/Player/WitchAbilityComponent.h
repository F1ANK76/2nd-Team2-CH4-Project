// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WitchAbilityComponent.generated.h"

class ABaseWitchAbility;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORIGINALSINPRJ_API UWitchAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWitchAbilityComponent();

	void CheckMoveable(const FVector2D& Value);

protected:
	virtual void BeginPlay() override;
	
private:
	void ActiveTimer();
	void AddLastAbilityToArray();
	void RemoveOldAbilityFromArray();
	void ClearLastAbilities();

public:
	// Move
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<ABaseWitchAbility> MoveAbility = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<ABaseWitchAbility> JumpAbility = nullptr;


	// Other
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<ABaseWitchAbility> GuardAbility = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<ABaseWitchAbility> RollAbility = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<ABaseWitchAbility> TauntAbility = nullptr;


	// Normal Attack
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<ABaseWitchAbility> NormalAttackAbility = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<ABaseWitchAbility> UpperAttackAbility = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<ABaseWitchAbility> LowerAttackAbility = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<ABaseWitchAbility> DashAttackAbility = nullptr;


	// Special Attack
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<ABaseWitchAbility> SpecialAttackAbility = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<ABaseWitchAbility> DropKickAttackAbility = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<ABaseWitchAbility> UppercutAttackAbility = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<ABaseWitchAbility> ChopAttackAbility = nullptr;

	
	// Skill Attack
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<ABaseWitchAbility> Skill1Ability = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<ABaseWitchAbility> Skill2Ability = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<ABaseWitchAbility> Skill3Ability = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<ABaseWitchAbility> Skill4Ability = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<ABaseWitchAbility> Skill5Ability = nullptr;

private:
	TArray<ABaseWitchAbility*> LastAbilities;
	TObjectPtr<ABaseWitchAbility> CurrentAbility = nullptr;

	FTimerHandle BufferTimer;
	float BufferActiveTime = 0.3f;

	bool bIsMoveable = true;
	bool bIsUseable = true;
	bool bIsLeft = true;
	float DirectionValue = 0.0f;

	/*TObjectPtr<ABaseWitchAbility> MoveAbility = nullptr;
	TObjectPtr<ABaseWitchAbility> JumpAbility = nullptr;
	TObjectPtr<ABaseWitchAbility> GuardAbility = nullptr;
	TObjectPtr<ABaseWitchAbility> RollAbility = nullptr;
	TObjectPtr<ABaseWitchAbility> TauntAbility = nullptr;

	TObjectPtr<ABaseWitchAbility> NormalAttackAbility = nullptr;
	TObjectPtr<ABaseWitchAbility> UpperAttackAbility = nullptr;
	TObjectPtr<ABaseWitchAbility> LowerAttackAbility = nullptr;
	TObjectPtr<ABaseWitchAbility> DashAttackAbility = nullptr;

	TObjectPtr<ABaseWitchAbility> SpecialAttackAbility = nullptr;
	TObjectPtr<ABaseWitchAbility> UppercutAttackAbility = nullptr;
	TObjectPtr<ABaseWitchAbility> ChopAttackAbility = nullptr;
	TObjectPtr<ABaseWitchAbility> DropkickAttackAbility = nullptr;

	TObjectPtr<ABaseWitchAbility> Skill1Ability = nullptr;
	TObjectPtr<ABaseWitchAbility> Skill2Ability = nullptr;
	TObjectPtr<ABaseWitchAbility> Skill3Ability = nullptr;
	TObjectPtr<ABaseWitchAbility> Skill4Ability = nullptr;
	TObjectPtr<ABaseWitchAbility> Skill5Ability = nullptr;*/
};
