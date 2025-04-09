// OriginalSinPrjGameInstance.cpp

#include "OriginalSinPrjGameInstance.h"

#include "UISubsystem.h"
#include "AudioSubsystem.h"
#include "DataSubsystem.h"

void UOriginalSinPrjGameInstance::Init()
{
    // 내부적으로 서브시스템 Initialize
    Super::Init();

    UISubsystem = GetSubsystem<UUISubsystem>();

    // 모든 객체 생성 이후에? 실행이 필요할 수 있음
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