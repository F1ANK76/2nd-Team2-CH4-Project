// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/WitchTypes.h"
#include "Player/Struct/AbilityDataBuffer.h"
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

	void CallMove(const FVector2D& Value);
	void CallNormalAttack();
	void CallSpecialAttack();
	void CallSkillAttack(int32 SkillNum);
	void CallJump();
	void CallHit(AActor* DamageCauser, float DamageValue);
	void CallBeginGuard();
	void CallKeepGuard();
	void CallEndGuard();
	void CallTaunt();

	void ResponseEndAnim();
	void PauseBufferTimer();
	void AddCurrentMana(float Value);
	void SetMaxMana(float Value);

protected:
	virtual void BeginPlay() override;

	ABaseWitchAbility* SpawnAbility(UClass* TargetClass);
	void ExcuteCurrentAbility();
	void ActiveTimer();
	void AddLastAbilityToArray();
	void RemoveOldAbilityFromArray();
	void ClearLastAbilities();

	void CallNormalAttackAtMove();
	void CallNormalAttackAtJump();
	void CallSpecialAttackAtMove();
	void CallSpecialAttackAtJump();
	void CallRoll(const FVector2D& DirectionVector);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<ABaseWitchAbility> HitAbilityClass = nullptr;


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
	UPROPERTY()
	TObjectPtr<ABaseWitch> ParentWitch = nullptr;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> ParentMovementComp = nullptr;

	UPROPERTY()
	FAbilityDataBuffer AbilityBuffer;

	FTimerHandle BufferTimer;

	float BufferActiveTime = 1.0f;
	bool bIsPlayingAnim = false;

	int32 MaxMana = 5;
	int32 CurrentMana = 5;

	TObjectPtr<ABaseWitchAbility> TempAbility = nullptr;

	UPROPERTY()
	TObjectPtr<ABaseWitchAbility> MoveAbility = nullptr;

	UPROPERTY()
	TObjectPtr<ABaseWitchAbility> JumpAbility = nullptr;

	UPROPERTY()
	TObjectPtr<ABaseWitchAbility> GuardAbility = nullptr;

	UPROPERTY()
	TObjectPtr<ABaseWitchAbility> RollAbility = nullptr;

	UPROPERTY()
	TObjectPtr<ABaseWitchAbility> TauntAbility = nullptr;

	UPROPERTY()
	TObjectPtr<ABaseWitchAbility> HitAbility = nullptr;


	UPROPERTY()
	TObjectPtr<ABaseWitchAbility> NormalAttackAbility = nullptr;

	UPROPERTY()
	TObjectPtr<ABaseWitchAbility> UpperAttackAbility = nullptr;

	UPROPERTY()
	TObjectPtr<ABaseWitchAbility> LowerAttackAbility = nullptr;

	UPROPERTY()
	TObjectPtr<ABaseWitchAbility> DashAttackAbility = nullptr;


	UPROPERTY()
	TObjectPtr<ABaseWitchAbility> SpecialAttackAbility = nullptr;

	UPROPERTY()
	TObjectPtr<ABaseWitchAbility> UppercutAttackAbility = nullptr;

	UPROPERTY()
	TObjectPtr<ABaseWitchAbility> ChopAttackAbility = nullptr;

	UPROPERTY()
	TObjectPtr<ABaseWitchAbility> DropkickAttackAbility = nullptr;


	UPROPERTY()
	TObjectPtr<ABaseWitchAbility> Skill1Ability = nullptr;

	UPROPERTY()
	TObjectPtr<ABaseWitchAbility> Skill2Ability = nullptr;

	UPROPERTY()
	TObjectPtr<ABaseWitchAbility> Skill3Ability = nullptr;

	UPROPERTY()
	TObjectPtr<ABaseWitchAbility> Skill4Ability = nullptr;

	UPROPERTY()
	TObjectPtr<ABaseWitchAbility> Skill5Ability = nullptr;
};
