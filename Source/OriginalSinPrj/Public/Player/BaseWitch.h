// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player/WitchTypes.h"
#include "BaseWitch.generated.h"

class AWitchController;
class UWitchAbilityComponent;
class UWitchAnimInstance;
struct FInputActionValue;

UCLASS()
class ORIGINALSINPRJ_API ABaseWitch : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseWitch();

	void SetWitchState(const EWitchStateType NewState);
	void SetWitchDirection();
	void PlayAnimation(UAnimMontage* Target);

	const EWitchStateType GetWitchState() const;
	const ECharacterType GetWitchType() const;

	void RequestMoveToAbility(float Value);
	void RequestUpDownToAbility(float Value);
	void RequestJumpToAbility();
	void RequestGuardToAbility();
	void RequestTauntToAbility();
	void RequestNormalAttackToAbility();
	void RequestSpecialAttackToAbility();
	void RequestSkillAttackToAbility(int32 Value);

protected:
	UFUNCTION(BlueprintCallable)
	void OnPressedMoveKey(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void OnPressedUpDownKey(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void OnPressedJumpKey(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void OnPressedNormalAttackKey(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void OnPressedSpecialAttackKey(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void OnPressedGuardKey(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void OnPressedTauntKey(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void OnPressedSkill1Key(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void OnPressedSkill2Key(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void OnPressedSkill3Key(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void OnPressedSkill4Key(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void OnPressedSkill5Key(const FInputActionValue& Value);


	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void InitAnimInstance();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWitchAbilityComponent> AbilityComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Clothes")
	TObjectPtr<USkeletalMeshComponent> DressMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Clothes")
	TObjectPtr<USkeletalMeshComponent> ShoesMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Clothes")
	TObjectPtr<USkeletalMeshComponent> StockingsMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UStaticMeshComponent> LeftHandItem = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UStaticMeshComponent> RightHandItem = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UStaticMeshComponent> HatItem = nullptr;

protected:
	TObjectPtr<AWitchController> WitchController = nullptr;
	TObjectPtr<UWitchAnimInstance> WitchAnimInstance = nullptr;
	TObjectPtr<UWitchAnimInstance> DressAnimInstance = nullptr;
	TObjectPtr<UWitchAnimInstance> StockingsAnimInstance = nullptr;
	TObjectPtr<UWitchAnimInstance> ShoesAnimInstance = nullptr;

	EWitchStateType CurrentState = EWitchStateType::Idle;
	ECharacterType WitchType = ECharacterType::Witch1; //
};
