// OriginalSinPrjGameInstance.cpp

#include "OriginalSinPrjGameInstance.h"

#include "UISubsystem.h"
#include "AudioSubsystem.h"
#include "DataSubsystem.h"
#include "GameInstance/LevelSubsystem.h"
#include "Player/Controller/WitchController.h"

void UOriginalSinPrjGameInstance::Init()
{
    // ���������� ����ý��� Initialize
    Super::Init();

    UISubsystem = GetSubsystem<UUISubsystem>();

    // ��� ��ü ���� ���Ŀ�? ������ �ʿ��� �� ����
    //if (UISubsystem)
    //{
    //    UISubsystem->ShowLevel(ELevelType::IntroLevel);
    //}

    AudioSubsystem = GetSubsystem<UAudioSubsystem>();

    DataSubsystem = GetSubsystem<UDataSubsystem>();

    LevelSubsystem = GetSubsystem<ULevelSubsystem>();
}

void UOriginalSinPrjGameInstance::SetOwningPlayerController(APlayerController* OwningPC)
{
    UE_LOG(LogTemp, Warning, TEXT("Set Owning Player"));
    OwningPlayer = OwningPC;
}

APlayerController* UOriginalSinPrjGameInstance::GetOwningPlayerController() const
{
    UE_LOG(LogTemp, Warning, TEXT("Get Owning Player"));
    return OwningPlayer;
}

void UOriginalSinPrjGameInstance::RequestOpenLevel(const FString& LevelName)
{
    if (!IsValid(OwningPlayer))
    {
        return;
    }

    if (!OwningPlayer->HasAuthority())
    {
        return;
    }

    if (IsValid(GetWorld()))
    {
        GetWorld()->ServerTravel(LevelName);
    }
}

void UOriginalSinPrjGameInstance::RequestOpenLevelByType(ELevelType Type, bool bIsSingle)
{
    checkf(IsValid(LevelSubsystem), TEXT("Level Sub is invalid"));

    if (bIsSingle)
    {
        LevelSubsystem->ResponseOpenSingleLevelByType(Type);
        return;
    }

    if (!IsValid(OwningPlayer))
    {
        return;
    }

    if (!OwningPlayer->HasAuthority())
    {
        return;
    }

    LevelSubsystem->ResponseServerTravelByType(Type);
}

void UOriginalSinPrjGameInstance::ResponseShowWidget()
{
    checkf(IsValid(UISubsystem), TEXT("UISubsystem is invalid"));
    
    UISubsystem->ShowLevelWidget(GetCurrentLevelType());

    checkf(IsValid(AudioSubsystem), TEXT("AudioSubsystem is invalid"));

    AudioSubsystem->PlayBGMSoundByLevel(GetCurrentLevelType());
}

const ELevelType UOriginalSinPrjGameInstance::GetCurrentLevelType()
{
    checkf(IsValid(LevelSubsystem), TEXT("LevelSubsystem is invalid"));
    LevelSubsystem->CompareMapName();
    return LevelSubsystem->GetCurrentLevel();
}
