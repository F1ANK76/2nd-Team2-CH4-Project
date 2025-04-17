// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "MapSelectWidget.generated.h"


class UButton;

UCLASS()
class ORIGINALSINPRJ_API UMapSelectWidget : public UBaseWidget
{
	GENERATED_BODY()

public:
	virtual void InitWidget(UUISubsystem* uiSubsystem) override;

private:
	UFUNCTION()
	void OnTileClickedFromTile(int32 TileIndex); // Ÿ�Ͽ��� ������� �̺�Ʈ ó��
	
	UFUNCTION()
	void OnClickedBack();
	//Open UI animation
	//close UI animation

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> UICloseButton = nullptr;
};
