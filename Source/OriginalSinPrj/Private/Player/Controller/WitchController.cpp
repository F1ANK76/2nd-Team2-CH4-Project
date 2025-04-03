// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Controller/WitchController.h"
#include <EnhancedInputSubsystems.h>
#include "InputMappingContext.h"

void AWitchController::BeginPlay()
{
	Super::BeginPlay();

	ULocalPlayer* LocalPlayer = GetLocalPlayer();

	if (IsValid(LocalPlayer))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

		if (IsValid(Subsystem))
		{
			if (IsValid(WitchIMC))
			{
				Subsystem->AddMappingContext(WitchIMC, 0);
			}
		}
	}
}
