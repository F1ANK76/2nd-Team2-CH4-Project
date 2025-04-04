#include "IntroWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UIntroWidget::NativeConstruct()
{
    Super::NativeConstruct();
    PlayFadeIn();
}

void UIntroWidget::PlayFadeIn()
{
    if (FadeAnimation)
    {
        PlayAnimation(FadeAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward);
        UE_LOG(LogTemp, Log, TEXT("Play Fade In"));
    }
}

void UIntroWidget::PlayFadeOut()
{
    if (FadeAnimation)
    {
        PlayAnimation(FadeAnimation, 0.0f, 1, EUMGSequencePlayMode::Reverse);
        UE_LOG(LogTemp, Log, TEXT("Play Fade Out"));
        this->RemoveFromParent();
        UE_LOG(LogTemp, Log, TEXT("Title Widget apply"));
        if (TitleWidgetClass)
        {
            TitleWidget = CreateWidget<UUserWidget>(GetWorld(), TitleWidgetClass);
            if (TitleWidget)
            {
                TitleWidget->AddToViewport();
                PlayFadeIn();
            }
        }
    }
}

void UIntroWidget::OnFadeOutFinished()
{
    this->RemoveFromParent();
    UE_LOG(LogTemp, Log, TEXT("Title Widget apply"));
    if (TitleWidgetClass)
    {
        TitleWidget = CreateWidget<UUserWidget>(GetWorld(), TitleWidgetClass);
        if (TitleWidget)
        {
            TitleWidget->AddToViewport();
            PlayFadeIn();
        }
    }
}
