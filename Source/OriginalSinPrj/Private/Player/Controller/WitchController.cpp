// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Controller/WitchController.h"
#include <EnhancedInputSubsystems.h>
#include "OriginalSinPrj/GameInstance/OriginalSinPrjGameInstance.h"
#include "OriginalSinPrj/Public/GameMode/CooperationGameMode.h"
#include "InputMappingContext.h"
#include "MenuGameMode.h"

void AWitchController::OnReadiedClient()
{
	if (IsLocalPlayerController())
	{
		RequestOnReadied();
	}
}

void AWitchController::OnUnreadiedClient()
{
	if (IsLocalPlayerController())
	{
		RequestUnReadied();
	}
}

void AWitchController::OnClickedStartButton()
{
	RequestStartGame();
}

void AWitchController::ResponseShowLevelWidget_Implementation()
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::ShowLevelWidget);
}

void AWitchController::RequestOnReadied_Implementation()
{
	if (!CheckValidOfMenuMode())
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Client Ready : %s"), *GetName());
	MenuMode->ReadyPlayer(this);
}

void AWitchController::RequestUnReadied_Implementation()
{
	if (!CheckValidOfMenuMode())
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Client Unready : %s"), *GetName());
	MenuMode->UnreadyPlayer(this);
}

void AWitchController::RequestStartGame_Implementation()
{
	if (!CheckValidOfMenuMode())
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Call Request Start Game"));
	MenuMode->StartMultiGame();
}

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

	if (CheckValidOfGameInstance())
	{
		LocalGameInstance->SetOwningPlayerController(this);
		LocalGameInstance->ResponseShowWidget();
	}
}

bool AWitchController::CheckValidOfGameMode()
{
	if (!HasAuthority())
	{
		return false;
	}

	GameMode = GetWorld()->GetAuthGameMode();

	if (!IsValid(GameMode))
	{
		return false;
	}

	return true;
}

bool AWitchController::CheckValidOfMenuMode()
{
	if (!CheckValidOfGameMode())
	{
		return false;
	}

	MenuMode = Cast<AMenuGameMode>(GameMode);

	if (!IsValid(MenuMode))
	{
		return false;
	}

	return true;
}

bool AWitchController::CheckValidOfGameInstance()
{
	if (!IsLocalController())
	{
		return false;
	}

	LocalGameInstance = GetGameInstance<UOriginalSinPrjGameInstance>();

	if (!IsValid(LocalGameInstance))
	{
		return false;
	}

	return true;;
}

void AWitchController::ShowLevelWidget(UWorld* LoadedWorld)
{
	if (!CheckValidOfGameInstance())
	{
		return;
	}

	LocalGameInstance->ResponseShowWidget();
}


void AWitchController::Server_SelectBuff_Implementation(int32 BuffIndex, FBuffInfo buff)
{
	if (AGameModeBase* GM = GetWorld()->GetAuthGameMode())
	{
		if (ACooperationGameMode* MyGM = Cast<ACooperationGameMode>(GM))
		{
			MyGM->ApplyBuffToPlayer(this, BuffIndex, buff); // or GetPawn(), or PlayerState ��
		}
	}
}