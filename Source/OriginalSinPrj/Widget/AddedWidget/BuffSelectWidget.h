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
	
	// ���� UI �ؽ�Ʈ + �̹���
	// ���� ���� ��ư

public:
    virtual void InitWidget(UUISubsystem* uiSubsystem) override;

    // �ܺο��� ���� 3�� ���� //�ڷ��� �ٲ���ҵ�
    void InitializeBuffs(const TArray<FBuffInfo>& InBuffs);

protected:
    UFUNCTION()
    void OnBuff1Clicked();
    UFUNCTION()
    void OnBuff2Clicked();
    UFUNCTION()
    void OnBuff3Clicked();

    UFUNCTION()
    void DeactivateOhterWidgets(int32 SelectedButtonIndex);

protected:
    // �� ī�� ������� (BindWidget���� BP ����)
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BuffName1;
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BuffDesc1;
    UPROPERTY(meta = (BindWidget))
    class UImage* BuffImage1;
    UPROPERTY(meta = (BindWidget))
    class UButton* BuffButton1;

    // 2�� ī��
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BuffName2;
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BuffDesc2;
    UPROPERTY(meta = (BindWidget))
    class UImage* BuffImage2;
    UPROPERTY(meta = (BindWidget))
    class UButton* BuffButton2;

    // 3�� ī��
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BuffName3;
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* BuffDesc3;
    UPROPERTY(meta = (BindWidget))
    class UImage* BuffImage3;
    UPROPERTY(meta = (BindWidget))
    class UButton* BuffButton3;

private:
    TArray<FBuffInfo> Buffs;

};
