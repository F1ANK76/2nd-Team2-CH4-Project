// OriginalSinPrjGameInstance.cpp

#include "OriginalSinPrjGameInstance.h"

#include "UISubsystem.h"
#include "AudioSubsystem.h"
#include "DataSubsystem.h"

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

    //if (AudioSubsystem)
    //{
    //    AudioSubsystem->PlayBGM(ESoundDataType::Level, static_cast<uint8>(ELevelSoundType::TitleSound));
    //}

    DataSubsystem = GetSubsystem<UDataSubsystem>();
}