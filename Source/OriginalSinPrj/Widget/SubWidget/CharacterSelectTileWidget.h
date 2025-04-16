// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "../BaseWidget.h"
#include "Player/WitchTypes.h"
#include "CharacterSelectTileWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterTileClicked, ECharacterType, TileIndex);

class UButton;

UCLASS()
class ORIGINALSINPRJ_API UCharacterSelectTileWidget : public UBaseWidget
{
	GENERATED_BODY()

	//�̺�Ʈ ������
public:
	virtual void InitWidget(UUISubsystem* uiSubsystem) override;
	void SetCharacterType(ECharacterType Type);
	void SetCharacterImage(UTexture2D* CharacterImg);

private:
	UFUNCTION()
	void HandleButtonClicked(); // ��ư�� ������ �Լ�

public:
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnCharacterTileClicked OnCharacterSelectTileClicked;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tile")
	int32 MyTileIndex;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CharacterName;

	UPROPERTY(meta = (BindWidget))
	class UImage* CharacterImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CharacterSelectButton;

	UPROPERTY()
	ECharacterType CharacterType = ECharacterType::Hand;

	//��ư �ޱ�
	//�̹��� UI
	//
};
