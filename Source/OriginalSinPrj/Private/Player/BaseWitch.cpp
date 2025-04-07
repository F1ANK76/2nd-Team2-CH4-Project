// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BaseWitch.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Player/Controller/WitchController.h"
#include "Player/WitchAnimInstance.h"
#include "Player/WitchAbilityComponent.h"
#include "Components/BoxComponent.h"

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

	ForwardDamager = CreateDefaultSubobject<UBoxComponent>(TEXT("Forward Damager"));
	BackDamager = CreateDefaultSubobject<UBoxComponent>(TEXT("Back Damager"));
	UpperDamager = CreateDefaultSubobject<UBoxComponent>(TEXT("Upper Damager"));
	LowerDamager = CreateDefaultSubobject<UBoxComponent>(TEXT("Lower Damager"));

	checkf(IsValid(GetMesh()), TEXT("Skeletal Mesh is invalid"));

	DressMesh->SetupAttachment(GetMesh());
	StockingsMesh->SetupAttachment(GetMesh());
	ShoesMesh->SetupAttachment(GetMesh());

	ForwardDamager->SetupAttachment(GetMesh());
	BackDamager->SetupAttachment(GetMesh());
	UpperDamager->SetupAttachment(GetMesh());
	LowerDamager->SetupAttachment(GetMesh());

	ForwardDamager->SetGenerateOverlapEvents(true);
	BackDamager->SetGenerateOverlapEvents(true);
	UpperDamager->SetGenerateOverlapEvents(true);
	LowerDamager->SetGenerateOverlapEvents(true);

	ForwardDamager->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BackDamager->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	UpperDamager->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	LowerDamager->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ABaseWitch::SetWitchState(const EWitchStateType NewState)
{
	CurrentState = NewState;
	
	/*if (IsValid(WitchAnimInstance))
	{
		WitchAnimInstance->SetAnimState(CurrentState);
	}

	if (IsValid(DressAnimInstance))
	{
		DressAnimInstance->SetAnimState(CurrentState);
	}

	if (IsValid(StockingsAnimInstance))
	{
		StockingsAnimInstance->SetAnimState(CurrentState);
	}

	if (IsValid(ShoesAnimInstance))
	{
		ShoesAnimInstance->SetAnimState(CurrentState);
	}*/
}

void ABaseWitch::SetWitchDirection(const FVector2D& DirectionVector)
{
	
}

void ABaseWitch::PlayAnimation(UAnimMontage* Target)
{
	if (IsValid(WitchAnimInstance))
	{
		WitchAnimInstance->Montage_Play(Target);
	}

	if (IsValid(DressAnimInstance))
	{
		DressAnimInstance->Montage_Play(Target);
	}

	if (IsValid(StockingsAnimInstance))
	{
		StockingsAnimInstance->Montage_Play(Target);
	}

	if (IsValid(ShoesAnimInstance))
	{
		ShoesAnimInstance->Montage_Play(Target);
	}
}

const EWitchStateType ABaseWitch::GetWitchState() const
{
	return CurrentState;
}

const ECharacterType ABaseWitch::GetWitchType() const
{
	return WitchType;
}

UBoxComponent* ABaseWitch::GetDamager(EDirectionType Target) const
{
	switch (Target)
	{
	case EDirectionType::Left:
		return BackDamager;
		break;

	case EDirectionType::Right:
		return ForwardDamager;
		break;

	case EDirectionType::Up:
		return UpperDamager;
		break;

	case EDirectionType::Down:
		return LowerDamager;
		break;

	default:
		checkNoEntry();
		break;
	}
	return nullptr;
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
	if (IsValid(AbilityComp))
	{
		AbilityComp->CheckMoveable(FVector2D(0, Value));
	}
}

void ABaseWitch::RequestJumpToAbility()
{
	if (IsValid(AbilityComp))
	{
		AbilityComp->CheckJumpable();
	}
}

void ABaseWitch::RequestGuardToAbility()
{
}

void ABaseWitch::RequestTauntToAbility()
{
}

void ABaseWitch::RequestNormalAttackToAbility()
{
	if (IsValid(AbilityComp))
	{
		AbilityComp->CheckAttackable(EAttackType::NormalAttack);
	}
}

void ABaseWitch::RequestSpecialAttackToAbility()
{
	if (IsValid(AbilityComp))
	{
		AbilityComp->CheckAttackable(EAttackType::SpecialAttack);
	}
}

void ABaseWitch::RequestSkillAttackToAbility(int32 Value)
{
	if (IsValid(AbilityComp))
	{
		AbilityComp->CheckSkillAttackable(Value);
	}
}

void ABaseWitch::RequestHitToAbility(const FVector& TargetPos)
{
	if (IsValid(AbilityComp))
	{
		AbilityComp->CheckHitable(TargetPos);
	}
}

void ABaseWitch::BeginPlay()
{
	Super::BeginPlay();
	
	InitAnimInstance();
}

float ABaseWitch::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!IsValid(DamageCauser))
	{
		return 0.0f;
	}

	RequestHitToAbility(DamageCauser->GetActorLocation());

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
