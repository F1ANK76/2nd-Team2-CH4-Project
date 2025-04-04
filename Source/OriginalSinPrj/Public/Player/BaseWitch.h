// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseWitch.generated.h"

class AWitchController;
struct FInputActionValue;

UCLASS()
class ORIGINALSINPRJ_API ABaseWitch : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseWitch();

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
	//virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:


protected:
	TObjectPtr<AWitchController> WitchController = nullptr;

	bool bIsJumpable = true;
};
