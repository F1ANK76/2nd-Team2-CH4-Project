// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "../AddedWidget/MapSelectWidget.h"
#include "MapSelectTileWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMapTileClicked, int32, TileIndex);

class UButton;

UCLASS()
class ORIGINALSINPRJ_API UMapSelectTileWidget : public UMapSelectWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

	//이벤트 디스패쳐
public:
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnMapTileClicked OnMapSelectTileClicked;

	UFUNCTION()
	void HandleButtonClicked(); // 버튼에 연결할 함수

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MapSelectButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tile")
	int32 MyTileIndex;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MapName;

	UPROPERTY(meta = (BindWidget))
	class UImage* MapImage;
	//닫기 UI button
	//UI Open animation
	//UI close Animation
};
