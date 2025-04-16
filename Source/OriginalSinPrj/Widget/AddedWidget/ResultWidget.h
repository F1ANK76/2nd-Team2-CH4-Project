// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "ResultWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class ORIGINALSINPRJ_API UResultWidget : public UBaseWidget
{
	GENERATED_BODY()

public:
	virtual void InitWidget(UUISubsystem* uiSubsystem) override;

	void InitResultWidgetData(
		float InPlayer1Damage,
		float InPlayer2Damage,
		int32 InPlayer1Death,
		int32 InPlayer2Death,
		int32 InPlayer1AttackCount,
		int32 InPlayer2AttackCount,
		float InStage1Timer,
		float InStage2Timer,
		float InStage3Timer,
		bool bStage1Started,
		bool bStage2Started,
		bool bStage3Started);

private:
	UFUNCTION()
	void OnClickedRetry();

	void UpdateResult(bool bIsClear);

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> MoveNextButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> MoveTitleButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> MoveNextButtonText;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> OpenAnim;


	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> Stage1Timer;
	
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> Stage2Timer;	
	
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> Stage3Timer;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> Player1Death;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> Player2Death;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> Player1AttackCount;
	
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> Player2AttackCount;


	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> Player1ReceivedDamage;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> Player2ReceivedDamage;

};
