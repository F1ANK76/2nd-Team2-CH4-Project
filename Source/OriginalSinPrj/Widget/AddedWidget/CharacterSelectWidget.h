// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "CharacterSelectWidget.generated.h"

class UButton;

UCLASS()
class ORIGINALSINPRJ_API UCharacterSelectWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnTileClickedFromTile(int32 TileIndex); // Ÿ�Ͽ��� ������� �̺�Ʈ ó��


	
	//UI Open animation
	//UI close Animation
};
