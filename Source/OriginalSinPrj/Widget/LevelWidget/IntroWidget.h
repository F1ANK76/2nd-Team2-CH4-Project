#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "IntroWidget.generated.h"

UCLASS()
class ORIGINALSINPRJ_API UIntroWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:

    virtual void NativeConstruct() override;
    
    UFUNCTION(BlueprintCallable)
    void PlayFadeIn();

    UFUNCTION(BlueprintCallable)
    void PlayFadeOut();
protected:
    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* FadeAnimation;

    UFUNCTION()
    void OnFadeOutFinished();

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> TitleWidgetClass;

    UPROPERTY()
    class UUserWidget* TitleWidget;
};
