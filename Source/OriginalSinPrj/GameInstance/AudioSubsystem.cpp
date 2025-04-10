// AudioSubsystem.cpp

#include "AudioSubsystem.h"
#include "AudioDataSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Struct/LevelAudioDataStruct.h"

#include "AudioDevice.h"

void UAudioSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//World = GetGameInstance()->GetWorld();

	AudioDataSettings = GetDefault<UAudioDataSettings>();

    LoadDataTables();
}

void UAudioSubsystem::LoadDataTables()
{
    if (AudioDataSettings)
    {
        if (AudioDataSettings->LevelSounds.IsValid())
        {
            LevelSoundTable = AudioDataSettings->LevelSounds.LoadSynchronous();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid LevelSoundTable"));
        }
    }
}

void UAudioSubsystem::PlaySounds(ESoundDataType SoundType, uint8 DetailSoundType)
{
    if (GetWorld())
    {
        UDataTable* TargetTable = nullptr;
        UEnum* EnumPtr = nullptr;

        switch (SoundType)
        {
            case ESoundDataType::Level:
                TargetTable = LevelSoundTable;
                EnumPtr = StaticEnum<ELevelSoundType>();
                break;

            default:
                UE_LOG(LogTemp, Warning, TEXT("Invalid SoundType"));
                return;
        }

        if (TargetTable)
        {
            FName RowName = FName(*EnumPtr->GetNameStringByValue(static_cast<int64>(DetailSoundType)));

            FString ContextString = TEXT("AudioSubsystem");

            const FLevelAudioDataStruct* FoundRow = TargetTable->FindRow<FLevelAudioDataStruct>(RowName, ContextString);

            if (FoundRow)
            {
                if (FoundRow->Sound.IsValid())
                {
                    USoundBase* Sound = FoundRow->Sound.LoadSynchronous();

                    if (Sound)
                    {
                        if (BgmComp && BgmComp->IsPlaying())
                        {
                            BgmComp->Stop();
                            UE_LOG(LogTemp, Warning, TEXT("Stop Sound"));
                        }

                        BgmComp = UGameplayStatics::CreateSound2D(GetWorld(), Sound, 1.0f);

                        if (BgmComp)
                        {
                            BgmComp->Play();
                        }
                        else
                        {
                            UE_LOG(LogTemp, Warning, TEXT("Invalid BgmComp"));
                        }
                    }
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Invalid FoundRow"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid TargetTable"));
        }
    }
}