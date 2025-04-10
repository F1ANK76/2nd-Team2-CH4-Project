// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "TitleWidget.generated.h"

class UButton;

UCLASS()
class ORIGINALSINPRJ_API UTitleWidget : public UBaseWidget
{
	GENERATED_BODY()


public:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> SinglePlayButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> TrainingButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> MultiPlayButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> OptionButton;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;
};
