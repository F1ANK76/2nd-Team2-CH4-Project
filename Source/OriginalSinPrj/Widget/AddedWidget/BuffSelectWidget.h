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

class UTextBlock;
class UImage;
class ABuffSelectManager;
class UButton;
class UDataTable;
class UCanvasPanel;

UCLASS()
class ORIGINALSINPRJ_API UBuffSelectWidget : public UBaseWidget
{
	GENERATED_BODY()


public:
    virtual void InitWidget(UUISubsystem* uiSubsystem) override;
    //virtual void NativeConstruct() override;

    void InitializeBuffs();
    TArray<FBuffInfo> BuffTable;

    UPROPERTY()	
    UDataTable* MyCBuffTable; //CSV파일과 연동하여 데이터 불러들이기.

    bool bIsHovered1 = false;
    bool bIsHovered2 = false;
    bool bIsHovered3 = false;


protected:
    UFUNCTION(BlueprintCallable)
    void OnBuff1Clicked();
    UFUNCTION(BlueprintCallable)
    void OnBuff2Clicked();
    UFUNCTION(BlueprintCallable)
    void OnBuff3Clicked();

    UFUNCTION(BlueprintCallable)
    void OnHoveredButton1();
    UFUNCTION(BlueprintCallable)
    void OnUnHoveredButton1();
    UFUNCTION(BlueprintCallable)
    void OnHoveredButton2();
    UFUNCTION(BlueprintCallable)
    void OnUnHoveredButton2();
    UFUNCTION(BlueprintCallable)
    void OnHoveredButton3();
    UFUNCTION(BlueprintCallable)
    void OnUnHoveredButton3();



    UFUNCTION()
    void DeactivateOhterWidgets(int32 SelectedButtonIndex, FBuffInfo buff);



protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> BuffName1;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> BuffImage1;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> BuffButton1;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> BuffName2;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> BuffImage2;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> BuffButton2;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> BuffName3;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> BuffImage3;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> BuffButton3;


    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCanvasPanel> Buff1InfoBox;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UTextBlock> Buff1InfoText;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCanvasPanel> Buff2InfoBox;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UTextBlock> Buff2InfoText;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UCanvasPanel> Buff3InfoBox;

    UPROPERTY(Meta = (BindWidget))
    TObjectPtr<UTextBlock> Buff3InfoText;

};
