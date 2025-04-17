// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "../BaseWidget.h"
#include "MapSelectTileWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMapTileClicked, int32, TileIndex);

class UButton;

UCLASS()
class ORIGINALSINPRJ_API UMapSelectTileWidget : public UBaseWidget
{
	GENERATED_BODY()
	

public:
	virtual void InitWidget(UUISubsystem* uiSubsystem) override;

	//�̺�Ʈ ������
public:
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnMapTileClicked OnMapSelectTileClicked;

	UFUNCTION()
	void HandleButtonClicked(); // ��ư�� ������ �Լ�

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MapSelectButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tile")
	int32 MyTileIndex;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MapName;

	UPROPERTY(meta = (BindWidget))
	class UImage* MapImage;
	//�ݱ� UI button
	//UI Open animation
	//UI close Animation
};
