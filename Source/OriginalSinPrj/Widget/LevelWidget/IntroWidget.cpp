#include "IntroWidget.h"
#include "Components/Button.h"
//#include "System/UIHandle.h"

void UIntroWidget::InitWidget(UUISubsystem* NewUIHandle)
{
    Super::InitWidget(NewUIHandle);

    //각 버튼에 클릭시 발동?될 함수 연결하기
    ToTitleButton->OnClicked.AddDynamic(this, &ThisClass::OnClickedMoveTitle);
}


void UIntroWidget::NativeConstruct()
{
    Super::NativeConstruct();
}