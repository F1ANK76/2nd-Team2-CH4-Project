// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "../AddedWidget/CharacterSelectWidget.h"
#include "CharacterSelectTileWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterTileClicked, int32, TileIndex);

class UButton;

UCLASS()
class ORIGINALSINPRJ_API UCharacterSelectTileWidget : public UCharacterSelectWidget
{
	GENERATED_BODY()
	
	virtual void InitWidget(UUISubsystem* uiHandle);
	virtual void NativeConstruct() override;
	//이벤트 디스패쳐
public:
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnCharacterTileClicked OnCharacterSelectTileClicked;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CharacterSelectButton;

	UFUNCTION()
	void HandleButtonClicked(); // 버튼에 연결할 함수

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tile")
	int32 MyTileIndex;

	//버튼 달기
	//이미지 UI
	//
};
