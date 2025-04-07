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
        UISubsystem->ShowLevel(ELevelType::TitleLevel);
    }

    AudioSubsystem = GetSubsystem<UAudioSubsystem>();

    DataSubsystem = GetSubsystem<UDataSubsystem>();
}