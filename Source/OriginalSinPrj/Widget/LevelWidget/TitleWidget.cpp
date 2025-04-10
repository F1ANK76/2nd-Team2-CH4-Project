// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"
#include "Components/Button.h"
#include "../GameInstance/UISubsystem.h"


void UTitleWidget::NativeConstruct()
{
    Super::NativeConstruct();
	//�� ��ư�� Ŭ���� �ߵ�?�� �Լ� �����ϱ�
    SinglePlayButton->OnClicked.RemoveDynamic(this, &ThisClass::OnClickedMoveSingleMode);
    SinglePlayButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveSingleMode);

    TrainingButton->OnClicked.RemoveDynamic(this, &ThisClass::OnClickedMoveTrainingMode);
    TrainingButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveTrainingMode);

    MultiPlayButton->OnClicked.RemoveDynamic(this, &ThisClass::OnClickedMoveMultiMode);
    MultiPlayButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveMultiMode);

    OptionButton->OnClicked.RemoveDynamic(this, &ThisClass::OnClickedOptionButton);
    OptionButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedOptionButton);

    QuitButton->OnClicked.RemoveDynamic(this, &ThisClass::OnClickedQuitGame);
    QuitButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedQuitGame);
}