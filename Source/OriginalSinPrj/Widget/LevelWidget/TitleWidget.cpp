// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"
#include "Components/Button.h"
#include "../GameInstance/UISubsystem.h"


void UTitleWidget::InitWidget(UUISubsystem* uiSubsystem)
{
    Super::InitWidget(uiSubsystem);

    SinglePlayButton->OnClicked.AddDynamic(this, &ThisClass::OnSelectedSingleMode);
    TrainingButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveTrainingMode);
    MultiPlayButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveMultiLobbyMode);
    OptionButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedOptionButton);
    QuitButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedQuitGame);
}

void UTitleWidget::OnSelectedSingleMode()
{
    checkf(IsValid(UIHandle), TEXT("UIHandle is invalid"));
    UIHandle->SetVisibilityWidget(false);
    OnClickedOpenWidget(EAddWidgetType::CharacterSelectWidget);

    PlayUIEffectSound(EUISfxSoundType::Click);
}
