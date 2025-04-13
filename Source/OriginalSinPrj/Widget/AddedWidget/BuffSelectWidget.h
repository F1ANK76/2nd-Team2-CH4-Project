// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "BuffSelectWidget.generated.h"


USTRUCT(BlueprintType)
struct FBuffInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText BuffName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText BuffDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* BuffIcon;
};



UCLASS()
class ORIGINALSINPRJ_API UBuffSelectWidget : public UBaseWidget
{
	GENERATED_BODY()
	
	// 버프 UI 텍스트 + 이미지
	// 버프 선택 버튼

public:
    // 외부에서 버프 3개 설정 //자료형 바꿔야할듯
    void InitializeBuffs(const TArray<FBuffInfo>& InBuffs);

protected:
    virtual void NativeConstruct() override;

    // 각 카드 구성요소 (BindWidget으로 BP 연결)
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BuffName1;
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BuffDesc1;
    UPROPERTY(meta = (BindWidget))
    class UImage* BuffImage1;
    UPROPERTY(meta = (BindWidget))
    class UButton* BuffButton1;

    // 2번 카드
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BuffName2;
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BuffDesc2;
    UPROPERTY(meta = (BindWidget))
    class UImage* BuffImage2;
    UPROPERTY(meta = (BindWidget))
    class UButton* BuffButton2;

    // 3번 카드
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BuffName3;
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BuffDesc3;
    UPROPERTY(meta = (BindWidget))
    class UImage* BuffImage3;
    UPROPERTY(meta = (BindWidget))
    class UButton* BuffButton3;

    UFUNCTION()
    void OnBuff1Clicked();
    UFUNCTION()
    void OnBuff2Clicked();
    UFUNCTION()
    void OnBuff3Clicked();

    UFUNCTION()
    void DeactivateOhterWidgets(int32 SelectedButtonIndex);

private:
    TArray<FBuffInfo> Buffs;

};
