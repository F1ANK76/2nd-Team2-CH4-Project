// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/WitchTypes.h"
#include "WitchAbilityComponent.generated.h"

class ABaseWitchAbility;
class ABaseWitch;
class UCharacterMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORIGINALSINPRJ_API UWitchAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWitchAbilityComponent();

	void CheckMoveable(const FVector2D& Value);
	void CheckAttackable(const EAttackType AttackType);
	void CheckSkillAttackable(int32 SkillNum);
	void CheckJumpable();

protected:
	virtual void BeginPlay() override;

	ABaseWitchAbility* SpawnAbility(UClass* TargetClass);
	void ExcuteCurrentAbility(const FVector2D& DirectionVector);
	void ActiveTimer();
	void AddLastAbilityToArray();
	void RemoveOldAbilityFromArray();
	void ClearLastAbilities();

	void ApplyNormalAttack();
	void ApplySpecialAttack();
	void ApplyJumpAttack();
	void ApplySkillAttack(int32 SkillNum);

	void CheckDirection();

public:
	// Move
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<ABaseWitchAbility> MoveAbilityClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<ABaseWitchAbility> JumpAbilityClass = nullptr;


	// Other
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<ABaseWitchAbility> GuardAbilityClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<ABaseWitchAbility> RollAbilityClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<ABaseWitchAbility> TauntAbilityClass = nullptr;


	// Normal Attack
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<ABaseWitchAbility> NormalAttackAbilityClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<ABaseWitchAbility> UpperAttackAbilityClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<ABaseWitchAbility> LowerAttackAbilityClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<ABaseWitchAbility> DashAttackAbilityClass = nullptr;


	// Special Attack
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<ABaseWitchAbility> SpecialAttackAbilityClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<ABaseWitchAbility> DropKickAttackAbilityClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<ABaseWitchAbility> UppercutAttackAbilityClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<ABaseWitchAbility> ChopAttackAbilityClass = nullptr;


	// Skill Attack
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<ABaseWitchAbility> Skill1AbilityClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<ABaseWitchAbility> Skill2AbilityClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<ABaseWitchAbility> Skill3AbilityClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<ABaseWitchAbility> Skill4AbilityClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<ABaseWitchAbility> Skill5AbilityClass = nullptr;

private:
	TArray<ABaseWitchAbility*> LastAbilities;

	TObjectPtr<ABaseWitchAbility> CurrentAbility = nullptr;
	TObjectPtr<ABaseWitch> ParentWitch = nullptr;
	TObjectPtr<UCharacterMovementComponent> ParentMovementComp = nullptr;

	FTimerHandle BufferTimer;
	float BufferActiveTime = 1.0f;

	bool bIsMoveable = true;
	bool bIsAttackable = true;
	bool bIsJumpable = true;
	
	EDirectionType DirectionType = EDirectionType::Left;

	int32 MaxMana = 5;
	int32 CurrentMana = 5;

	FVector2D Direction = FVector2D::ZeroVector;

	TObjectPtr<ABaseWitchAbility> TempAbility = nullptr;

	TObjectPtr<ABaseWitchAbility> MoveAbility = nullptr;
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
	TObjectPtr<ABaseWitchAbility> Skill5Ability = nullptr;
};
