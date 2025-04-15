// OriginalSinPrjGameInstance.cpp

#include "OriginalSinPrjGameInstance.h"

#include "UISubsystem.h"
#include "AudioSubsystem.h"
#include "DataSubsystem.h"
#include "GameInstance/MyOnlineSubsystem.h"

void UOriginalSinPrjGameInstance::Init()
{
    // ���������� ����ý��� Initialize
    Super::Init();

    UISubsystem = GetSubsystem<UUISubsystem>();

    // ��� ��ü ���� ���Ŀ�? ������ �ʿ��� �� ����
    if (UISubsystem)
    {
        UISubsystem->ShowLevel(ELevelType::IntroLevel);
    }

    AudioSubsystem = GetSubsystem<UAudioSubsystem>();

    DataSubsystem = GetSubsystem<UDataSubsystem>();

    MyOnlineSubsystem = GetSubsystem<UMyOnlineSubsystem>();
}