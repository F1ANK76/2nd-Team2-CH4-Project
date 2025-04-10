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

protected:
	virtual void NativeConstruct() override;
	virtual void InitWidget(UUISubsystem* NewUIHandle) override;

public:
	UFUNCTION()
	void OnTileClickedFromTile(int32 TileIndex); // Ÿ�Ͽ��� ������� �̺�Ʈ ó��
	
	//Open UI animation
	//close UI animation
};
