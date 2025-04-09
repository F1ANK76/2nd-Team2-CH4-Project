#include "IntroWidget.h"
#include "Components/Button.h"
//#include "System/UIHandle.h"

void UIntroWidget::InitWidget(UUISubsystem* NewUIHandle)
{
    Super::InitWidget(NewUIHandle);

    //�� ��ư�� Ŭ���� �ߵ�?�� �Լ� �����ϱ�
    ToTitleButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveTitle);
}


void UIntroWidget::NativeConstruct()
{
    Super::NativeConstruct();
}