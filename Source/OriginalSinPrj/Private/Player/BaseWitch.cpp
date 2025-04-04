// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BaseWitch.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Player/Controller/WitchController.h"
#include "Player/WitchAnimInstance.h"
#include "Player/WitchAbilityComponent.h"

ABaseWitch::ABaseWitch()
{
	PrimaryActorTick.bCanEverTick = false;

	DressMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Dress"));
	StockingsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Stokings"));
	ShoesMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoes"));

	/*LeftHandItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftHand"));
	RightHandItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightHand"));
	HatItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hat"));*/

	AbilityComp = CreateDefaultSubobject<UWitchAbilityComponent>(TEXT("Ability Component"));

	checkf(IsValid(GetMesh()), TEXT("Skeletal Mesh is invalid"));

	DressMesh->SetupAttachment(GetMesh());
	StockingsMesh->SetupAttachment(GetMesh());
	ShoesMesh->SetupAttachment(GetMesh());
	
	InitAnimInstance();
}

void ABaseWitch::SetWitchState(const EWitchStateType NewState)
{
	CurrentState = NewState;
	
	if (IsValid(WitchAnimInstance))
	{
		WitchAnimInstance->SetAnimState(NewState);
	}

	if (IsValid(DressAnimInstance))
	{
		DressAnimInstance->SetAnimState(NewState);
	}

	if (IsValid(StockingsAnimInstance))
	{
		StockingsAnimInstance->SetAnimState(NewState);
	}

	if (IsValid(ShoesAnimInstance))
	{
		ShoesAnimInstance->SetAnimState(NewState);
	}
}

void ABaseWitch::SetWitchDirection()
{
}

void ABaseWitch::RequestMoveToAbility(float Value)
{
	if (IsValid(AbilityComp))
	{
		AbilityComp->CheckMoveable(FVector2D(Value, 0));
	}
}

void ABaseWitch::RequestUpDownToAbility(float Value)
{
	if (Value >= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("On Pressed Up Key"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("On Pressed Down Key"));
	}
}

void ABaseWitch::RequestJumpToAbility()
{
	Jump();
}

void ABaseWitch::RequestGuardToAbility()
{
}

void ABaseWitch::RequestTauntToAbility()
{
}

void ABaseWitch::RequestNormalAttackToAbility()
{
}

void ABaseWitch::RequestSpecialAttackToAbility()
{
}

void ABaseWitch::RequestSkillAttackToAbility(int32 Value)
{
}

void ABaseWitch::BeginPlay()
{
	Super::BeginPlay();
	
}

float ABaseWitch::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

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

void ABaseWitch::InitAnimInstance()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (IsValid(AnimInstance))
	{
		WitchAnimInstance = Cast<UWitchAnimInstance>(AnimInstance);
	}

	AnimInstance = DressMesh->GetAnimInstance();

	if (IsValid(AnimInstance))
	{
		DressAnimInstance = Cast<UWitchAnimInstance>(AnimInstance);
	}

	AnimInstance = StockingsMesh->GetAnimInstance();

	if (IsValid(AnimInstance))
	{
		StockingsAnimInstance = Cast<UWitchAnimInstance>(AnimInstance);
	}

	AnimInstance = ShoesMesh->GetAnimInstance();

	if (IsValid(AnimInstance))
	{
		ShoesAnimInstance = Cast<UWitchAnimInstance>(AnimInstance);
	}

	AnimInstance = nullptr;
}

void ABaseWitch::OnPressedMoveKey(const FInputActionValue& Value)
{
	float MoveValue = Value.Get<float>();

	RequestMoveToAbility(MoveValue);
}

void ABaseWitch::OnPressedUpDownKey(const FInputActionValue& Value)
{
	float UpDownValue = Value.Get<float>();

	RequestUpDownToAbility(UpDownValue);
}

void ABaseWitch::OnPressedJumpKey(const FInputActionValue& Value)
{
	RequestJumpToAbility();
}

void ABaseWitch::OnPressedNormalAttackKey(const FInputActionValue& Value)
{
	RequestNormalAttackToAbility();
}

void ABaseWitch::OnPressedSpecialAttackKey(const FInputActionValue& Value)
{
	RequestSpecialAttackToAbility();
}

void ABaseWitch::OnPressedGuardKey(const FInputActionValue& Value)
{
	RequestGuardToAbility();
}

void ABaseWitch::OnPressedTauntKey(const FInputActionValue& Value)
{
	RequestTauntToAbility();
}

void ABaseWitch::OnPressedSkill1Key(const FInputActionValue& Value)
{
	RequestSkillAttackToAbility(0);
}

void ABaseWitch::OnPressedSkill2Key(const FInputActionValue& Value)
{
	RequestSkillAttackToAbility(1);
}

void ABaseWitch::OnPressedSkill3Key(const FInputActionValue& Value)
{
	RequestSkillAttackToAbility(2);
}

void ABaseWitch::OnPressedSkill4Key(const FInputActionValue& Value)
{
	RequestSkillAttackToAbility(3);
}

void ABaseWitch::OnPressedSkill5Key(const FInputActionValue& Value)
{
	RequestSkillAttackToAbility(4);
}
