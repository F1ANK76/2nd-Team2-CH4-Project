// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WitchAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

void UWitchAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* OwnPawn = TryGetPawnOwner();

	if (IsValid(OwnPawn))
	{
		ACharacter* OwnCharacter = Cast<ACharacter>(OwnPawn);

		if (IsValid(OwnCharacter))
		{
			MovementComp = OwnCharacter->GetCharacterMovement();
		}
	}
}

void UWitchAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (IsValid(MovementComp))
	{
		Speed = MovementComp->Velocity.Size();
	}
}
