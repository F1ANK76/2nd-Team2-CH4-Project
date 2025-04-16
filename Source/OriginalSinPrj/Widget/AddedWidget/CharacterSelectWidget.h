// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "CharacterSelectWidget.generated.h"

class UButton;
class UCharacterSelectTileWidget;

UCLASS()
class ORIGINALSINPRJ_API UCharacterSelectWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void InitWidget(UUISubsystem* uiHandle) override;

private:
	UFUNCTION()
	void OnTileClickedFromTile(int32 TileIndex); // Ÿ�Ͽ��� ������� �̺�Ʈ ó��

	UFUNCTION()
	void OnClickedClose();

	void InitDelegate();
	void InitCharacterTiles();

private:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> UICloseButton = nullptr;

	UPROPERTY()
	TArray<UCharacterSelectTileWidget*> Tiles;

	//UI Open animation
	//UI close Animation
};
