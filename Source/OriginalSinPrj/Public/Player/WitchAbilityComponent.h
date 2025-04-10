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
	void CallHit(AActor* DamageCauser);
	void CallBeginGuard();
	void CallKeepGuard();
	void CallEndGuard();
	void CallTaunt();

	void ResponseEndAnim();
	void PauseBufferTimer();

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

	TObjectPtr<ABaseWitchAbility> MoveAbility = nullptr;
	TObjectPtr<ABaseWitchAbility> JumpAbility = nullptr;
	TObjectPtr<ABaseWitchAbility> GuardAbility = nullptr;
	TObjectPtr<ABaseWitchAbility> RollAbility = nullptr;
	TObjectPtr<ABaseWitchAbility> TauntAbility = nullptr;
	TObjectPtr<ABaseWitchAbility> HitAbility = nullptr;

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
