// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WitchController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class ORIGINALSINPRJ_API AWitchController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> WitchIMC = nullptr;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = "Input")
	TObjectPtr<UInputAction> UpDownAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = "Input")
	TObjectPtr<UInputAction> GuardAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = "Input")
	TObjectPtr<UInputAction> NormalAttackAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = "Input")
	TObjectPtr<UInputAction> SpecialAttackAction = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = "Input")
	TObjectPtr<UInputAction> TauntAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = "Input")
	TObjectPtr<UInputAction> SkillAction1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = "Input")
	TObjectPtr<UInputAction> SkillAction2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = "Input")
	TObjectPtr<UInputAction> SkillAction3 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = "Input")
	TObjectPtr<UInputAction> SkillAction4 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintreadOnly, Category = "Input")
	TObjectPtr<UInputAction> SkillAction5 = nullptr;
};
