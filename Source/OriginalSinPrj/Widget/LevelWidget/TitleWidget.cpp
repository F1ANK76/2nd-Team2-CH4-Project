// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"
#include "Components/Button.h"
//#include "System/UIHandle.h"

void UTitleWidget::InitWidget(UUIHandle* NewUIHandle)
{
	Super::InitWidget(NewUIHandle);

	//�� ��ư�� Ŭ���� �ߵ�?�� �Լ� �����ϱ�
	SinglePlayButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveNext);
	TrainingButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveNext);
	MultiPlayButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveNext);
	OptionButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedOptionButton);
	QuitButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedQuitGame);
}

void UTitleWidget::OnClickedOptionButton()
{
	//OnClickedOpenWidget(EWidgetType::OptionWidget);
}