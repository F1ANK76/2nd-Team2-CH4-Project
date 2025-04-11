// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "PlayerStateWidget.generated.h"


class UTextBlock;
class UImage;
class UProgressBar;

USTRUCT(BlueprintType)
struct FPlayerUIData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString PlayerName;

    UPROPERTY(BlueprintReadWrite)
    UTexture2D* CharacterImage;

    UPROPERTY(BlueprintReadWrite)
    float CurrentHP;
    UPROPERTY(BlueprintReadWrite)
    float MaxHP;

    UPROPERTY(BlueprintReadWrite)
    float CurrentMana;
    UPROPERTY(BlueprintReadWrite)
    float MaxMana;

    UPROPERTY(BlueprintReadWrite)
    float CurrentSpecial;
    UPROPERTY(BlueprintReadWrite)
    float MaxSpecial;

    UPROPERTY(BlueprintReadWrite)
    int32 AirbornePercent;

    UPROPERTY(BlueprintReadWrite)
    int32 PlayerLevel;

    UPROPERTY(BlueprintReadWrite)
    float CurrentEXP;

    UPROPERTY(BlueprintReadWrite)
    float MaxEXP;
};





UCLASS()
class ORIGINALSINPRJ_API UPlayerStateWidget : public UBaseWidget
{
	GENERATED_BODY()
	

public:
    void InitPlayerState(const FPlayerUIData& Data);
    void UpdateStatus(const FPlayerUIData& Data); // 실시간 업데이트 용

protected:
    // 위젯 참조
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* PlayerNameText;

    UPROPERTY(meta = (BindWidget))
    class UImage* CharacterImageUI;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HPBar;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* ManaBar;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* EXPBar;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* AirbornePercentText;
};
