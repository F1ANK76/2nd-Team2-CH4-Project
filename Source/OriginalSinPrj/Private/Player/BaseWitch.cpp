// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BaseWitch.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Player/Controller/WitchController.h"
#include "Player/WitchAnimInstance.h"
#include "Player/WitchAbilityComponent.h"
#include "Player/BuffComponent.h"
#include "Components/BoxComponent.h"
#include <Kismet/GameplayStatics.h>
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine/DamageEvents.h"
#include "Player/Projectile/BaseProjectile.h"
#include "Components/CapsuleComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

ABaseWitch::ABaseWitch()
{
	PrimaryActorTick.bCanEverTick = false;

	DressMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Dress"));
	StockingsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Stokings"));
	ShoesMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoes"));

	LeftHandItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftHand"));
	RightHandItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightHand"));
	HatItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hat"));
	FootItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Foot"));

	AbilityComp = CreateDefaultSubobject<UWitchAbilityComponent>(TEXT("Ability Component"));
	BuffComp = CreateDefaultSubobject<UBuffComponent>(TEXT("Buff Component"));
	PerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Perceptions Stimuli Source Component"));

	LeftHandDamager = CreateDefaultSubobject<UBoxComponent>(TEXT("Left Hand Damager"));
	RightHandDamager = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Hand Damager"));

	LeftHandEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Left Hand Effect"));
	RightHandEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Right Hand Effect"));

	checkf(IsValid(GetMesh()), TEXT("Skeletal Mesh is invalid"));

	MainMesh = GetMesh();
	HitCollision = Cast<UCapsuleComponent>(GetRootComponent());

	checkf(IsValid(HitCollision), TEXT("Root Component is not Capsule Component"));

	DressMesh->SetupAttachment(MainMesh);
	StockingsMesh->SetupAttachment(MainMesh);
	ShoesMesh->SetupAttachment(MainMesh);

	LeftHandItem->SetupAttachment(MainMesh, (FName)"LeftHand");
	RightHandItem->SetupAttachment(MainMesh, (FName)"RightHand");
	HatItem->SetupAttachment(MainMesh, (FName)"Head");
	FootItem->SetupAttachment(MainMesh, (FName)"Foot");

	LeftHandEffect->SetupAttachment(LeftHandItem);
	RightHandEffect->SetupAttachment(RightHandItem);

	LeftHandDamager->SetupAttachment(LeftHandItem);
	RightHandDamager->SetupAttachment(RightHandItem);

	LeftHandDamager->SetGenerateOverlapEvents(true);
	RightHandDamager->SetGenerateOverlapEvents(true);

	LeftHandDamager->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RightHandDamager->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	Tags.Add((FName)"Player");
}

void ABaseWitch::SetWitchState_Implementation(const EWitchStateType NewState)
{
	CurrentState = NewState;
}

void ABaseWitch::PlayAnimation_Implementation(UAnimMontage* Target)
{
	if (!IsValid(Target))
	{
		//EndAnimNotify();
		return;
	}

	if (IsLocallyControlled())
	{
		RequestPauseTimer();
	}

	if (IsValid(WitchAnimInstance))
	{
		WitchAnimInstance->Montage_Play(Target);
		CheckAttackSpeed(WitchAnimInstance, Target);
	}

	if (IsValid(DressAnimInstance))
	{
		DressAnimInstance->Montage_Play(Target);
		CheckAttackSpeed(DressAnimInstance, Target);
	}

	if (IsValid(StockingsAnimInstance))
	{
		StockingsAnimInstance->Montage_Play(Target);
		CheckAttackSpeed(StockingsAnimInstance, Target);
	}

	if (IsValid(ShoesAnimInstance))
	{
		ShoesAnimInstance->Montage_Play(Target);
		CheckAttackSpeed(ShoesAnimInstance, Target);
	}
}

void ABaseWitch::StopAnimation_Implementation(UAnimMontage* Target)
{
	if (!IsValid(Target))
	{
		return;
	}

	if (!WitchAnimInstance->Montage_IsPlaying(Target))
	{
		return;
	}

	if (IsValid(WitchAnimInstance))
	{
		WitchAnimInstance->Montage_Stop(0.1f);
	}

	if (IsValid(DressAnimInstance))
	{
		DressAnimInstance->Montage_Stop(0.1f);
	}

	if (IsValid(StockingsAnimInstance))
	{
		StockingsAnimInstance->Montage_Stop(0.1f);
	}

	if (IsValid(ShoesAnimInstance))
	{
		ShoesAnimInstance->Montage_Stop(0.1f);
	}
}

void ABaseWitch::PlayEffect_Implementation(EEffectVisibleType Type)
{
	switch (Type)
	{
	case EEffectVisibleType::Left:
		LeftHandEffect->Activate(true);
		break;

	case EEffectVisibleType::Right:
		RightHandEffect->Activate(true);
		break;

	case EEffectVisibleType::Both:
		LeftHandEffect->Activate(true);
		RightHandEffect->Activate(true);
		break;
	}
}

void ABaseWitch::StopEffect_Implementation()
{
	LeftHandEffect->Deactivate();
	RightHandEffect->Deactivate();
}

void ABaseWitch::SetDamagerEnabledByType(EEffectVisibleType DamagerType, bool bIsActive)
{
	switch (DamagerType)
	{
	case EEffectVisibleType::Left:
		SetDamagerEnabled(LeftHandDamager, bIsActive);
		break;

	case EEffectVisibleType::Right:
		SetDamagerEnabled(RightHandDamager, bIsActive);
		break;

	case EEffectVisibleType::Both:
		SetDamagerEnabled(LeftHandDamager, bIsActive);
		SetDamagerEnabled(RightHandDamager, bIsActive);
		break;
	}
}

void ABaseWitch::SetDamagerEnabled(UBoxComponent* Target, bool bIsActive)
{
	if (bIsActive)
	{
		Target->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		Target->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABaseWitch::CheckAttackSpeed(UAnimInstance* TargetInstance, UAnimMontage* TargetMontage)
{
	if (CurrentState == EWitchStateType::Attack)
	{
		float OriginRate = TargetInstance->Montage_GetPlayRate(TargetMontage);
		OriginRate *= AttackSpeed;
		TargetInstance->Montage_SetPlayRate(TargetMontage);
	}
}

bool ABaseWitch::CompareColorIndex(AActor* DamageCauser)
{
	if (!bIsColorMode)
	{
		return true;
	}
	
	if (!DamageCauser->FindFunction("GetColorIndex"))
	{
		return true;
	}

	ABaseWitch* CauserWitch = Cast<ABaseWitch>(DamageCauser);

	if (IsValid(CauserWitch))
	{
		if (CauserWitch->GetColorIndex() == bIsFirstIndex)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	return true;
}

void ABaseWitch::SetMeshResponseToChanel_Implementation(ECollisionChannel Chanel, ECollisionResponse Response)
{
	MainMesh->SetCollisionResponseToChannel(Chanel, Response);
	HitCollision->SetCollisionResponseToChannel(Chanel, Response);
}

void ABaseWitch::PlayMelleAttack(EEffectVisibleType Type, float DamageValue)
{
	Damage = DamageValue;
	SetDamagerEnabledByType(Type, true);
}

void ABaseWitch::StopMelleAttack()
{
	SetDamagerEnabledByType(EEffectVisibleType::Both, false);
	bIsActivedOverlap = false;
	Damage = 0.0f;
}

void ABaseWitch::ApplyAttack(AActor* Target, float ApplyValue)
{
	if (!IsValid(Target))
	{
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("%s : Apply Attack. Target = %s, Value = %f"), *GetName(), *Target->GetName(), ApplyValue);
	//UGameplayStatics::ApplyDamage(Target, ApplyValue, GetController(), this, UDamageType::StaticClass());

	float RealDamage = ApplyValue + BuffComp->GetBuffData().AddedKnockGauge;

	float Result = Target->TakeDamage(RealDamage, FDamageEvent(), GetController(), this);

	if (Result > 0)
	{
		IncreaseCurrentMana();
	}
}

void ABaseWitch::EndAnimNotify()
{
	if (IsLocallyControlled())
	{
		RequestEndedAnim();
	}
}

void ABaseWitch::PauseTimer()
{
	AbilityComp->PauseBufferTimer();
}

void ABaseWitch::RequestPauseTimer_Implementation()
{
	AbilityComp->PauseBufferTimer();
}

const EWitchStateType ABaseWitch::GetWitchState() const
{
	return CurrentState;
}

const ECharacterType ABaseWitch::GetWitchType() const
{
	return WitchType;
}

const FVector ABaseWitch::GetHeadLocation() const
{
	return HatItem->GetComponentLocation();
}

const FVector ABaseWitch::GetFootLocation() const
{
	return FootItem->GetComponentLocation();
}

const UBuffComponent* ABaseWitch::GetBuffComponent() const
{
	return BuffComp;
}

void ABaseWitch::SetPlayerLevel(int32 LevelValue)
{
	CharacterBuffer.PlayerLevel = FMath::Clamp(LevelValue, 0, 5);
	CharacterBuffer.MaxMana = FMath::Clamp(LevelValue, 0, 5);
	OnChangedState.Broadcast(CharacterBuffer);
}

void ABaseWitch::SetCharacterLifePoint(int32 LifeValue)
{
	CharacterBuffer.LifePoint = LifeValue;
	OnChangedState.Broadcast(CharacterBuffer);
}

void ABaseWitch::IncreaseLifePoint()
{
	++CharacterBuffer.LifePoint;
	OnChangedState.Broadcast(CharacterBuffer);
}

void ABaseWitch::DecreaseLifePoint()
{
	CharacterBuffer.LifePoint = FMath::Clamp(--CharacterBuffer.LifePoint, 0, 10);
	OnChangedState.Broadcast(CharacterBuffer);
}

void ABaseWitch::SetCharacterLevel(int32 LevelValue)
{
	CharacterBuffer.PlayerLevel = LevelValue;
	OnChangedState.Broadcast(CharacterBuffer);
}

void ABaseWitch::IncreaseExp()
{
	CharacterBuffer.CurrentEXP = CharacterBuffer.CurrentEXP + AddedExpValue;

	if (CharacterBuffer.CurrentEXP >= CharacterBuffer.MaxEXP)
	{
		if (CharacterBuffer.PlayerLevel < 5)
		{
			CharacterBuffer.CurrentEXP -= CharacterBuffer.MaxEXP;
			++CharacterBuffer.PlayerLevel;
		}
		else
		{
			CharacterBuffer.CurrentEXP = 100;
		}
	}

	OnChangedState.Broadcast(CharacterBuffer);
}

void ABaseWitch::IncreaseCircle()
{
	++CharacterBuffer.MaxMana;
	OnChangedState.Broadcast(CharacterBuffer);
}

void ABaseWitch::IncreaseCurrentMana()
{
	float MaxValue = CharacterBuffer.MaxMana;
	float AddedValue = BuffComp->GetBuffData().AddedMana;

	CharacterBuffer.CurrentMana = FMath::Clamp(CharacterBuffer.CurrentMana + AddedValue, 0, MaxValue);
	OnChangedState.Broadcast(CharacterBuffer);
}

void ABaseWitch::AddKnockGauge(float KnockValue)
{
	CharacterBuffer.AirbornePercent += KnockValue;
	OnChangedState.Broadcast(CharacterBuffer);
}

void ABaseWitch::InitCharacterState()
{
	CharacterBuffer.MaxHP = 100;
	CharacterBuffer.MaxMana = 0;
	CharacterBuffer.CurrentEXP = 0;
	CharacterBuffer.PlayerLevel = 0;

	ResetCharacterState();
}

void ABaseWitch::ResetCharacterState()
{
	AbilityComp->ResetAbility();

	CharacterBuffer.CurrentHP = CharacterBuffer.MaxHP;
	CharacterBuffer.AirbornePercent = 0;
	CharacterBuffer.CurrentMana = 0;
	
	OnChangedState.Broadcast(CharacterBuffer);
}

const float ABaseWitch::GetCurrentMana() const
{
	return CharacterBuffer.CurrentMana;
}

const bool ABaseWitch::GetColorIndex() const
{
	return bIsFirstIndex;
}

void ABaseWitch::SetColorMode(bool Value)
{
	bIsColorMode = Value;
}

void ABaseWitch::SetColorIndex(bool Value)
{
	bIsFirstIndex = Value;
}

void ABaseWitch::RequestMoveToAbility_Implementation(float Value)
{
	if (IsValid(AbilityComp))
	{
		AbilityComp->CallMove(FVector2D(Value, 0));
	}
}

void ABaseWitch::RequestEndMoveToAbility_Implementation()
{
	if (CurrentState == EWitchStateType::Move)
	{
		SetWitchState(EWitchStateType::Idle);
	}
}

void ABaseWitch::RequestUpDownToAbility_Implementation(float Value)
{
	if (IsValid(AbilityComp))
	{
		AbilityComp->CallMove(FVector2D(0, Value));
	}
}

void ABaseWitch::RequestJumpToAbility_Implementation()
{
	if (IsValid(AbilityComp))
	{
		AbilityComp->CallJump();
	}
}

void ABaseWitch::RequestExcuteGuardToAbility_Implementation()
{
	if (IsValid(AbilityComp))
	{
		AbilityComp->CallBeginGuard();
	}
}

void ABaseWitch::RequestContinueGuardToAbility_Implementation()
{
	if (IsValid(AbilityComp))
	{
		AbilityComp->CallKeepGuard();
	}
}

void ABaseWitch::RequestUndoGuardToAbility_Implementation()
{
	if (IsValid(AbilityComp))
	{
		AbilityComp->CallEndGuard();
	}
}

void ABaseWitch::RequestTauntToAbility_Implementation()
{
	if (IsValid(AbilityComp))
	{
		AbilityComp->CallTaunt();
	}
}

void ABaseWitch::RequestNormalAttackToAbility_Implementation()
{
	if (IsValid(AbilityComp))
	{
		AbilityComp->CallNormalAttack();
	}
}

void ABaseWitch::RequestSpecialAttackToAbility_Implementation()
{
	if (IsValid(AbilityComp))
	{
		AbilityComp->CallSpecialAttack();
	}
}

void ABaseWitch::RequestSkillAttackToAbility_Implementation(int32 Value)
{
	if (IsValid(AbilityComp))
	{
		AbilityComp->CallSkillAttack(Value);
	}
}

void ABaseWitch::RequestHitToAbility_Implementation(AActor* DamageCauser, float DamageValue)
{
	if (IsValid(AbilityComp))
	{
		AbilityComp->CallHit(DamageCauser, DamageValue);
	}
}

void ABaseWitch::RequestEndedAnim_Implementation()
{
	if (IsValid(AbilityComp))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Request Ended Anim"));
		AbilityComp->ResponseEndAnim();
	}
}

void ABaseWitch::BeginPlay()
{
	Super::BeginPlay();
	
	InitAnimInstance();
	StopEffect();
	StopMelleAttack();

	LeftHandDamager->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	RightHandDamager->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);

	SetPlayerLevel(5); //Test
}

void ABaseWitch::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsActivedOverlap)
	{
		return;
	}

	if (!IsValid(OtherActor))
	{
		return;
	}

	if (OtherActor == this)
	{
		return;
	}

	if (OtherActor->GetClass() == ABaseProjectile::StaticClass())
	{
		return;
	}

	bIsActivedOverlap = true;
	ApplyAttack(OtherActor, Damage);
}

float ABaseWitch::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!HasAuthority())
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s : Has not Authority"), *GetName());
		return 0.0f;
	}

	if (!IsValid(DamageCauser))
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s : Take Damage. But DamageCauser is invalid"), *GetName());
		return 0.0f;
	}

	//UE_LOG(LogTemp, Warning, TEXT("%s : TakeDamage. Causer = %s, Value = %f"), *GetName(), *DamageCauser->GetName(), DamageAmount);

	if (CompareColorIndex(DamageCauser))
	{
		AbilityComp->CallHit(DamageCauser, DamageAmount);
		return 1.0f;
	}

	//RequestHitToAbility(DamageCauser);

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
			EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Completed, this, &ThisClass::OnEndPressedMoveKey);
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
			EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Started, this, &ThisClass::OnBeginPressedGuardKey);
			EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Triggered, this, &ThisClass::OnPressedGuardKey);
			EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Completed, this, &ThisClass::OnEndPressedGuardKey);
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
	
	if (CurrentState == EWitchStateType::Idle || CurrentState == EWitchStateType::Move)
	{
		AddMovementInput(GetActorRightVector(), FMath::Abs(MoveValue));
	}

	RequestMoveToAbility(MoveValue);
}

void ABaseWitch::OnEndPressedMoveKey(const FInputActionValue& Value)
{
	RequestEndMoveToAbility();
}

void ABaseWitch::OnPressedUpDownKey(const FInputActionValue& Value)
{
	float UpDownValue = Value.Get<float>();

	RequestUpDownToAbility(UpDownValue);
}

void ABaseWitch::OnPressedJumpKey(const FInputActionValue& Value)
{
	if (CurrentState == EWitchStateType::Idle || CurrentState == EWitchStateType::Move)
	{
		Jump();
	}

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

void ABaseWitch::OnBeginPressedGuardKey(const FInputActionValue& Value)
{
	RequestExcuteGuardToAbility();
}

void ABaseWitch::OnPressedGuardKey(const FInputActionValue& Value)
{
	RequestContinueGuardToAbility();
}

void ABaseWitch::OnEndPressedGuardKey(const FInputActionValue& Value)
{
	RequestUndoGuardToAbility();
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


