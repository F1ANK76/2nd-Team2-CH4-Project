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
	void OnTileClickedFromTile(int32 TileIndex); // 타일에서 날라오는 이벤트 처리


	
	//UI Open animation
	//UI close Animation
};
