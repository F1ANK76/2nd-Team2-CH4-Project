// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WitchAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Player/BaseWitch.h"


void UWitchAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* OwnPawn = TryGetPawnOwner();

	if (IsValid(OwnPawn))
	{
		OwnCharacter = Cast<ABaseWitch>(OwnPawn);

		if (IsValid(OwnCharacter))
		{
			MovementComp = OwnCharacter->GetCharacterMovement();
			CharacterType = OwnCharacter->GetWitchType();
		}
	}
}

void UWitchAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (IsValid(MovementComp))
	{
		FVector Velocity = MovementComp->Velocity;
		FVector Width = FVector(Velocity.X, Velocity.Y, 0);
		Speed = Width.Size();

		CharacterState = OwnCharacter->GetWitchState();

		bIsInAir = MovementComp->IsFalling();
	}
}
