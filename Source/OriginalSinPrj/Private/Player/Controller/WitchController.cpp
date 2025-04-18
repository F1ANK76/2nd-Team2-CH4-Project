// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Controller/WitchController.h"
#include <EnhancedInputSubsystems.h>
#include "OriginalSinPrj/GameInstance/OriginalSinPrjGameInstance.h"
#include "OriginalSinPrj/GameInstance/UISubsystem.h"
#include "OriginalSinPrj/Public/GameMode/CooperationGameMode.h"
#include "InputMappingContext.h"
#include "MenuGameMode.h"
#include "GameMode/MultiBattleGameMode.h"
#include "OriginalSinPrj/GameInstance/UISettings.h"

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
	auto TryShow = [this]()
	{
		if (GetWorld() && !GetWorld()->bIsTearingDown)
		{
			if (UOriginalSinPrjGameInstance* GI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
				if (UUISubsystem* Sub = GI->GetSubsystem<UUISubsystem>())
					if (Sub->CurrentActiveWidget)
					{
						ShowLevelWidget(GetWorld());
						return true;
					}
		}
		return false;
	};

	// 즉시 시도
	if (!TryShow())
	{
		// 0.1초 후에 한 번 더 재시도
		FTimerHandle RetryHandle;
		GetWorldTimerManager().SetTimer(RetryHandle, FTimerDelegate::CreateLambda([TryShow]()
		{
			TryShow();
		}), 0.1f, false);
	}
	
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

void AWitchController::Client_CreateBuffWidget_Implementation()
{
	if (AMultiBattleGameState* GM = Cast<AMultiBattleGameState>(GetWorld()->GetGameState()))
	{
		GM->CreateBuffSelectUI(this);
	}
}


void AWitchController::Server_SelectBuff_Implementation(int32 BuffIndex, EBuffType buff)
{
	if (AGameModeBase* GM = GetWorld()->GetAuthGameMode())
	{
		if (ACooperationGameMode* MyGM = Cast<ACooperationGameMode>(GM))
		{
			MyGM->ApplyBuffToPlayer(this, BuffIndex, buff); // or GetPawn(), or PlayerState ��
		}
		if (AMultiBattleGameMode* MyGM = Cast<AMultiBattleGameMode>(GM))
		{
			MyGM->ApplyBuffToBothPlayer(); // or GetPawn(), or PlayerState ��
		}
	}
}
