// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player/WitchTypes.h"
#include "Player/Struct/CharacterStateBuffer.h"
#include "Delegates/DelegateCombinations.h"
#include "OriginalSinPrj/GameInstance/EnumSet.h"
#include "BaseWitch.generated.h"

class AWitchController;
class UWitchAbilityComponent;
class UWitchAnimInstance;
class UBoxComponent;
class UNiagaraComponent;
class UBuffComponent;
class UAIPerceptionStimuliSourceComponent;
class UAudioComponent;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedCharacterState, const FCharacterStateBuffer&, CharacterStateBuffer);

UCLASS()
class ORIGINALSINPRJ_API ABaseWitch : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseWitch();

	UFUNCTION(NetMulticast, Reliable)
	void SetWitchState(const EWitchStateType NewState);

	UFUNCTION(NetMulticast, Unreliable)
	void ResponsePlayAnimation(UAnimMontage* Target, float SpeedValue);

	UFUNCTION(NetMulticast, Unreliable)
	void StopAnimation(UAnimMontage* Target);

	UFUNCTION(NetMulticast, Unreliable)
	void PlayEffect(EEffectVisibleType Type);

	UFUNCTION(NetMulticast, Unreliable)
	void StopEffect();

	UFUNCTION(NetMulticast, Reliable)
	void SetMeshResponseToChanel(ECollisionChannel Chanel, ECollisionResponse Response);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void RequestMoveToAbility(float Value);

	UFUNCTION(Server, Unreliable)
	void RequestEndMoveToAbility();

	UFUNCTION(Server, Unreliable)
	void RequestUpDownToAbility(float Value);

	UFUNCTION(Server, Unreliable)
	void RequestJumpToAbility();

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void RequestExcuteGuardToAbility();

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void RequestContinueGuardToAbility();

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void RequestUndoGuardToAbility();

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void RequestTauntToAbility();

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void RequestNormalAttackToAbility();

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void RequestSpecialAttackToAbility();

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void RequestSkillAttackToAbility(int32 Value);

	UFUNCTION(Server, Unreliable)
	void RequestHitToAbility(AActor* DamageCauser, float DamageValue);

	UFUNCTION(Server, Reliable)
	void RequestEndedAnim();

	UFUNCTION(Server, Reliable)
	void RequestPauseTimer();


	UFUNCTION(BlueprintCallable)
	const float GetCurrentMana() const;

	UFUNCTION()
	const bool GetColorIndex() const;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void PlayAnimation(UAnimMontage* Target);
	void PlayMelleAttack(EEffectVisibleType Type, float DamageValue);
	void StopMelleAttack();

	void ApplyAttack(AActor* Target, float ApplyValue);
	void EndAnimNotify();
	void PauseTimer();
	void OnOverlapedDeathZone();

	const EWitchStateType GetWitchState() const;
	const ECharacterType GetWitchType() const;
	const FVector GetHeadLocation() const;
	const FVector GetFootLocation() const;
	AActor* GetLastDamageCasuser() const;
	UAudioComponent* GetAudioComponent() const;

	void ResponseSelectedBuff(EBuffType TargetType);
	void SetCharacterLifePoint(int32 LifeValue);
	void IncreaseLifePoint();
	void DecreaseLifePoint();

	void SetCharacterLevel(int32 LevelValue);
	void SetPlayerLevel(int32 LevelValue);
	void IncreaseExp();

	void IncreaseCircle();
	void IncreaseCurrentMana();

	void ApplyDamage(float DamageValue, AActor* DamageCauser);

	void InitCharacterState();
	void ResetCharacterState();

	void SetHpMode(bool Value);
	void SetColorMode(bool Value);
	void SetColorIndex(bool Value);

	void RequestDieToGameMode();

protected:
	UFUNCTION(BlueprintCallable)
	void OnPressedMoveKey(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void OnEndPressedMoveKey(const FInputActionValue& Value);

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

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void InitAnimInstance();
	void SetDamagerEnabledByType(EEffectVisibleType DamagerType, bool bIsActive);
	void SetDamagerEnabled(UBoxComponent* Target, bool bIsActive);
	void CheckAttackSpeed(UAnimInstance* TargetInstance, UAnimMontage* TargetMontage, float SpeedValue);
	void CheckDie();
	bool CheckAvoid();
	bool CompareColorIndex(AActor* DamageCauser);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWitchAbilityComponent> AbilityComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBuffComponent> BuffComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAudioComponent> AudioComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAIPerceptionStimuliSourceComponent> PerceptionStimuliSource = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCapsuleComponent> HitCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> MainMesh = nullptr;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UStaticMeshComponent> FootItem = nullptr;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<UNiagaraComponent> LeftHandEffect = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<UNiagaraComponent> RightHandEffect = nullptr;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damager")
	TObjectPtr<UBoxComponent> LeftHandDamager = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damager")
	TObjectPtr<UBoxComponent> RightHandDamager = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ECharacterType WitchType = ECharacterType::Hand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	TObjectPtr<UAnimMontage> DieMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float AddedExpValue = 10.0f;

	UPROPERTY()
	FOnChangedCharacterState OnChangedState;

protected:
	UPROPERTY()
	FCharacterStateBuffer CharacterBuffer;

	UPROPERTY()
	TObjectPtr<AActor> LastDamageCauser = nullptr;

	TObjectPtr<AWitchController> WitchController = nullptr;
	TObjectPtr<UWitchAnimInstance> WitchAnimInstance = nullptr;
	TObjectPtr<UWitchAnimInstance> DressAnimInstance = nullptr;
	TObjectPtr<UWitchAnimInstance> StockingsAnimInstance = nullptr;
	TObjectPtr<UWitchAnimInstance> ShoesAnimInstance = nullptr;

	EWitchStateType CurrentState = EWitchStateType::Idle;

	bool bIsActivedOverlap = false;
	bool bIsColorMode = false;
	bool bIsFirstIndex = false;
	bool bIsHpMode = false;

	float Damage = 0.0f;
};
