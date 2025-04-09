// AudioSubsystem.cpp

#include "AudioSubsystem.h"
#include "AudioDataSettings.h"
#include "Kismet/GameplayStatics.h"

#include "AudioDevice.h"

void UAudioSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//World = GetGameInstance()->GetWorld();

	AudioDataSettings = GetDefault<UAudioDataSettings>();
}

void UAudioSubsystem::PlaySounds(ESoundDataType SoundType, uint8 DetailSoundType)
{
    if (GetWorld())
    {
        const TMap<uint8, TSoftObjectPtr<USoundBase>>* TargetSound = nullptr;

        switch (SoundType)
        {
            case ESoundDataType::Level:
                TargetSound = &AudioDataSettings->LevelSounds;
                break;

            case ESoundDataType::Monster:
                TargetSound = &AudioDataSettings->MonsterSounds;
                break;

            case ESoundDataType::Character:
                TargetSound = &AudioDataSettings->CharacterSounds;
                break;

            case ESoundDataType::Boss:
                TargetSound = &AudioDataSettings->BossSounds;
                break;

            default:
                UE_LOG(LogTemp, Warning, TEXT("Invalid SoundType"));
                return;
        }

        if (TargetSound && TargetSound->Contains(DetailSoundType))
        {
            TSoftObjectPtr<USoundBase> SoundAsset = (*TargetSound)[DetailSoundType];

            if (USoundBase* Sound = SoundAsset.LoadSynchronous())
            {
                if (BgmComp && BgmComp->IsPlaying())
                {
                    BgmComp->Stop();
                    UE_LOG(LogTemp, Warning, TEXT("Stop Sound"));
                }
                
                BgmComp = UGameplayStatics::CreateSound2D(GetWorld(), Sound, 1.0f);

                if (BgmComp)
                {
                    //BgmComp->bAutoDestroy = false;

                    BgmComp->Play();
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to Create BgmComp"));
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid DetailSoundType"));
        }
    }
}