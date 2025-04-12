// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player/WitchTypes.h"
#include "BaseWitch.generated.h"

class AWitchController;
class UWitchAbilityComponent;
class UWitchAnimInstance;
class UBoxComponent;
class UNiagaraComponent;
struct FInputActionValue;

UCLASS()
class ORIGINALSINPRJ_API ABaseWitch : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseWitch();

	UFUNCTION(NetMulticast, Unreliable)
	void SetWitchState(const EWitchStateType NewState);

	UFUNCTION(NetMulticast, Unreliable)
	void SetWitchDirection(const FVector2D& DirectionVector);

	UFUNCTION(NetMulticast, Unreliable)
	void PlayAnimation(UAnimMontage* Target);

	UFUNCTION(NetMulticast, Unreliable)
	void StopAnimation(UAnimMontage* Target);

	UFUNCTION(NetMulticast, Unreliable)
	void PlayEffect(EEffectVisibleType Type);

	UFUNCTION(NetMulticast, Unreliable)
	void StopEffect();

	void PlayEffect_Implementation(EEffectVisibleType Type);

	void ApplyAttack(AActor* Target, float ApplyValue);
	void EndAnimNotify();

	const EWitchStateType GetWitchState() const;
	const ECharacterType GetWitchType() const;
	const FVector GetHeadLocation() const;
	UBoxComponent* GetDamager(EDirectionType Target) const;

	UFUNCTION(Server, Unreliable)
	void RequestMoveToAbility(float Value);

	UFUNCTION(Server, Unreliable)
	void RequestUpDownToAbility(float Value);

	UFUNCTION(Server, Unreliable)
	void RequestJumpToAbility();

	UFUNCTION(Server, Unreliable)
	void RequestExcuteGuardToAbility();

	UFUNCTION(Server, Unreliable)
	void RequestContinueGuardToAbility();

	UFUNCTION(Server, Unreliable)
	void RequestUndoGuardToAbility();

	UFUNCTION(Server, Unreliable)
	void RequestTauntToAbility();

	UFUNCTION(Server, Unreliable)
	void RequestNormalAttackToAbility();

	UFUNCTION(Server, Unreliable)
	void RequestSpecialAttackToAbility();

	UFUNCTION(Server, Unreliable)
	void RequestSkillAttackToAbility(int32 Value);

	UFUNCTION(Server, Unreliable)
	void RequestHitToAbility(AActor* DamageCauser);

	UFUNCTION(Server, UnReliable)
	void RequestEndedAnim();

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
	void OnBeginPressedGuardKey(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void OnPressedGuardKey(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void OnEndPressedGuardKey(const FInputActionValue& Value);

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


	

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void BeginPlay() override;
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


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<UNiagaraComponent> LeftHandEffect = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<UNiagaraComponent> RightHandEffect = nullptr;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damager")
	TObjectPtr<UBoxComponent> LeftHandDamager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damager")
	TObjectPtr<UBoxComponent> RightHandDamager = nullptr;

protected:
	TObjectPtr<AWitchController> WitchController = nullptr;
	TObjectPtr<UWitchAnimInstance> WitchAnimInstance = nullptr;
	TObjectPtr<UWitchAnimInstance> DressAnimInstance = nullptr;
	TObjectPtr<UWitchAnimInstance> StockingsAnimInstance = nullptr;
	TObjectPtr<UWitchAnimInstance> ShoesAnimInstance = nullptr;

	EWitchStateType CurrentState = EWitchStateType::Idle;
	ECharacterType WitchType = ECharacterType::Witch1; //
};
