// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BaseWitch.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Player/Controller/WitchController.h"

ABaseWitch::ABaseWitch()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseWitch::BeginPlay()
{
	Super::BeginPlay();
	
}

//void ABaseWitch::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void ABaseWitch::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (!IsValid(EnhancedInputComponent))
	{
		return;
	}

	if (!IsValid(GetController()))
	{
		return;
	}

	WitchController = Cast<AWitchController>(GetController());
	
	if (IsValid(WitchController))
	{
		UInputAction* TargetAction = WitchController->MoveAction;

		if (IsValid(TargetAction))
		{
			EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Triggered, this, &ThisClass::OnPressedMoveKey);
		}

		TargetAction = WitchController->UpDownAction;

		if (IsValid(TargetAction))
		{
			EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Triggered, this, &ThisClass::OnPressedUpDownKey);
		}

		TargetAction = WitchController->JumpAction;

		if (IsValid(TargetAction))
		{
			EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Triggered, this, &ThisClass::OnPressedJumpKey);
		}

		TargetAction = WitchController->NormalAttackAction;

		if (IsValid(TargetAction))
		{
			EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Triggered, this, &ThisClass::OnPressedNormalAttackKey);
		}

		TargetAction = WitchController->SpecialAttackAction;

		if (IsValid(TargetAction))
		{
			EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Triggered, this, &ThisClass::OnPressedSpecialAttackKey);
		}

		TargetAction = WitchController->GuardAction;

		if (IsValid(TargetAction))
		{
			EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Triggered, this, &ThisClass::OnPressedGuardKey);
		}

		TargetAction = WitchController->TauntAction;

		if (IsValid(TargetAction))
		{
			EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Triggered, this, &ThisClass::OnPressedTauntKey);
		}

		TargetAction = WitchController->SkillAction1;

		if (IsValid(TargetAction))
		{
			EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Triggered, this, &ThisClass::OnPressedSkill1Key);
		}

		TargetAction = WitchController->SkillAction2;

		if (IsValid(TargetAction))
		{
			EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Triggered, this, &ThisClass::OnPressedSkill2Key);
		}

		TargetAction = WitchController->SkillAction3;

		if (IsValid(TargetAction))
		{
			EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Triggered, this, &ThisClass::OnPressedSkill3Key);
		}

		TargetAction = WitchController->SkillAction4;

		if (IsValid(TargetAction))
		{
			EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Triggered, this, &ThisClass::OnPressedSkill4Key);
		}

		TargetAction = WitchController->SkillAction5;

		if (IsValid(TargetAction))
		{
			EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Triggered, this, &ThisClass::OnPressedSkill5Key);
		}
	}
}

void ABaseWitch::OnPressedMoveKey(const FInputActionValue& Value)
{
	float MoveValue = Value.Get<float>();

	AddMovementInput(GetActorRightVector(), -MoveValue);
}

void ABaseWitch::OnPressedUpDownKey(const FInputActionValue& Value)
{
	float UpDownValue = Value.Get<float>();

	if (UpDownValue >= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("On Pressed Up Key"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("On Pressed Down Key"));
	}
}

void ABaseWitch::OnPressedJumpKey(const FInputActionValue& Value)
{
	Jump();
}

void ABaseWitch::OnPressedNormalAttackKey(const FInputActionValue& Value)
{

}

void ABaseWitch::OnPressedSpecialAttackKey(const FInputActionValue& Value)
{

}

void ABaseWitch::OnPressedGuardKey(const FInputActionValue& Value)
{

}

void ABaseWitch::OnPressedTauntKey(const FInputActionValue& Value)
{

}

void ABaseWitch::OnPressedSkill1Key(const FInputActionValue& Value)
{

}

void ABaseWitch::OnPressedSkill2Key(const FInputActionValue& Value)
{

}

void ABaseWitch::OnPressedSkill3Key(const FInputActionValue& Value)
{

}

void ABaseWitch::OnPressedSkill4Key(const FInputActionValue& Value)
{

}

void ABaseWitch::OnPressedSkill5Key(const FInputActionValue& Value)
{

}
