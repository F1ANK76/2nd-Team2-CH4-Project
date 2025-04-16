// AudioSubsystem.cpp

#include "AudioSubsystem.h"
#include "AudioDataSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Struct/LevelAudioDataStruct.h"
#include "Struct/UISfxAudioDataStruct.h"
#include "Struct/MonsterAudioDataStruct.h"
#include "Struct/CharacterAudioDataStruct.h"
#include "Struct/BossAudioDataStruct.h"
#include "AudioDevice.h"

void UAudioSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	AudioDataSettings = GetDefault<UAudioDataSettings>();

    LoadDataTables();
}

void UAudioSubsystem::LoadDataTables()
{
    if (AudioDataSettings)
    {
        if (!AudioDataSettings->UISfxSounds.IsNull())
        {
            UISfxSoundTable = AudioDataSettings->UISfxSounds.LoadSynchronous();
        }

        if (!AudioDataSettings->MonsterSounds.IsNull())
        {
            MonsterSoundTable = AudioDataSettings->MonsterSounds.LoadSynchronous();
        }

        if (!AudioDataSettings->CharacterSounds.IsNull())
        {
            CharacterSoundTable = AudioDataSettings->CharacterSounds.LoadSynchronous();
        }

        if (!AudioDataSettings->BossSounds.IsNull())
        {
            BossSoundTable = AudioDataSettings->BossSounds.LoadSynchronous();
        }
    }
}

void UAudioSubsystem::CheckLoadedLevelSound()
{
    if (!IsValid(AudioDataSettings))
    {
        return;
    }

    if (AudioDataSettings->LevelSounds.IsNull())
    {
        return;
    }

    if (!IsValid(LevelSoundTable))
    {
        LevelSoundTable = AudioDataSettings->LevelSounds.LoadSynchronous();
    }
}

void UAudioSubsystem::PlayBGMByLevelType(ELevelType LevelType)
{
    CheckLoadedLevelSound();

    ELevelSoundType SoundType = ELevelSoundType::TitleSound;

    switch (LevelType)
    {
    case ELevelType::IntroLevel:
        SoundType = ELevelSoundType::IntroSound;
        break;

    case ELevelType::TitleLevel:
        SoundType = ELevelSoundType::TitleSound;
        break;

    case ELevelType::TrainingLevel:
        SoundType = ELevelSoundType::TrainingSound;
        break;

    case ELevelType::FarmingLevel:
        SoundType = ELevelSoundType::FarmingSound;
        break;

    case ELevelType::SingleLevel:
        SoundType = ELevelSoundType::SingleSound;
        break;

    case ELevelType::MatchLevel:
        SoundType = ELevelSoundType::MatchSound;
        break;

    case ELevelType::LobbyLevel:
        SoundType = ELevelSoundType::LobbySound;
        break;

    case ELevelType::MultiLevel:
        SoundType = ELevelSoundType::MultiSound;
        break;

    case ELevelType::CooperationLevel:
        SoundType = ELevelSoundType::CooperationSound;
        break;
    }

    PlayBGM(SoundType);
}

void UAudioSubsystem::PlayBGM(ELevelSoundType SoundType)
{
    if (GetWorld())
    {
        UEnum* EnumPtr = StaticEnum<ELevelSoundType>();

        if (LevelSoundTable)
        {
            FName RowName = FName(*EnumPtr->GetNameStringByValue(static_cast<int64>(SoundType)));

            const FLevelAudioDataStruct* FoundRow = LevelSoundTable->FindRow<FLevelAudioDataStruct>(RowName, TEXT("PlayBGM AudioSubsystem"));

            if (FoundRow)
            {
                if (!FoundRow->Sound.IsNull())
                {
                    USoundBase* Sound = FoundRow->Sound.LoadSynchronous();

                    if (Sound)
                    {
                        if (BgmComp && BgmComp->IsPlaying())
                        {
                            BgmComp->Stop();
                            UE_LOG(LogTemp, Warning, TEXT("Stop BGM Sound"));
                        }

                        BgmComp = UGameplayStatics::CreateSound2D(GetWorld(), Sound, MasterVolume);

                        if (BgmComp)
                        {
                            BgmComp->Play();
                            UE_LOG(LogTemp, Warning, TEXT("Play BGM Sound"));
                        }
                    }
                }
            }
        }
    }
}

void UAudioSubsystem::PlaySFX(ESfxSoundType SoundType, uint8 DetailSoundType, FVector Location)
{
    switch (SoundType)
    {
        case ESfxSoundType::UI:
            PlaySFXByType<EUISfxSoundType, FUISfxAudioDataStruct>(GetWorld(), UISfxSoundTable, DetailSoundType, FVector(0.f, 0.f, 0.f));
            break;

        case ESfxSoundType::Monster:
            PlaySFXByType<EMonsterSoundType, FMonsterAudioDataStruct>(GetWorld(), MonsterSoundTable, DetailSoundType, Location);
            break;

        case ESfxSoundType::Character:
            PlaySFXByType<ECharacterSoundType, FCharacterAudioDataStruct>(GetWorld(), CharacterSoundTable, DetailSoundType, Location);
            break;

        case ESfxSoundType::Boss:
            PlaySFXByType<EBossSoundType, FBossAudioDataStruct>(GetWorld(), BossSoundTable, DetailSoundType, Location);
            break;

        default:
            UE_LOG(LogTemp, Warning, TEXT("Invalid SFX type"));
            return;
    }
}

void UAudioSubsystem::SetAndApplyMasterVolume(float NewVolume)
{
    MasterVolume = FMath::Clamp(NewVolume, 0.0f, 1.0f);

    if (BgmComp)
    {
        if (MasterVolume != 0)
        {
            BgmComp->SetVolumeMultiplier(MasterVolume);
            BgmComp->SetPaused(false);
        }
        else
        {
            BgmComp->SetPaused(true);
        }
    }
}