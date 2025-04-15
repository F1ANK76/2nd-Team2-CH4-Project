// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WitchController.generated.h"

class UInputMappingContext;
class UInputAction;
class UOriginalSinPrjGameInstance;
class AMenuGameMode;

UCLASS()
class ORIGINALSINPRJ_API AWitchController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void OnReadiedClient();
	void OnUnreadiedClient();
	void OnClickedStartButton();

	UFUNCTION(Client, Reliable)
	void ResponseShowLevelWidget();

protected:
	UFUNCTION(Server, Reliable)
	void RequestOnReadied();

	UFUNCTION(Server, Reliable)
	void RequestUnReadied();

	UFUNCTION(Server, Reliable)
	void RequestStartGame();

	virtual void BeginPlay() override;
	
	bool CheckValidOfGameMode();
	bool CheckValidOfMenuMode();
	bool CheckValidOfGameInstance();

	void ShowLevelWidget(UWorld* LoadedWorld);

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


protected:
	UPROPERTY()
	TObjectPtr<UOriginalSinPrjGameInstance> LocalGameInstance = nullptr;

	UPROPERTY()
	TObjectPtr<AGameModeBase> GameMode = nullptr;

	UPROPERTY()
	TObjectPtr<AMenuGameMode> MenuMode = nullptr;
};
