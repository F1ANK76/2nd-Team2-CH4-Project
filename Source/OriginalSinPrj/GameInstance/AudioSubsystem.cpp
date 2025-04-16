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

void UAudioSubsystem::PlayBGMSoundByLevel(ELevelType Type)
{
    ELevelSoundType SoundType = ELevelSoundType::TitleSound;

    switch (Type)
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

    PlayBGMSound(SoundType);
}

void UAudioSubsystem::PlayBGMSound(ELevelSoundType Type)
{
    if (!CheckValidOfBgmSource(Type))
    {
        return;
    }

    if (!IsValid(BgmSoundMap[Type]))
    {
        return;
    }

    if (!IsValid(BgmComp))
    {
        BgmComp = UGameplayStatics::CreateSound2D(GetGameInstance(), BgmSoundMap[Type], BgmVolume);
        BgmComp->bAutoDestroy = false;
    }

    if (BgmComp->IsPlaying())
    {
        BgmComp->Stop();
    }

    BgmComp->SetSound(BgmSoundMap[Type]);
    BgmComp->Play();
}

void UAudioSubsystem::PlayUISound(EUISfxSoundType Type)
{
    if (!CheckValidOfUISoundSource(Type))
    {
        return;
    }

    if (!IsValid(UISoundMap[Type]))
    {
        return;
    }

    if (!IsValid(UIAudioComp))
    {
        UIAudioComp = UGameplayStatics::CreateSound2D(GetGameInstance(), UISoundMap[Type], EffectVolume);
        UIAudioComp->bAutoDestroy = false;
    }

    if (UIAudioComp->IsPlaying())
    {
        UIAudioComp->Stop();
    }

    UIAudioComp->SetSound(UISoundMap[Type]);
    UIAudioComp->Play();
}

TArray<FCharacterAudioDataStruct*>& UAudioSubsystem::GetCharacterSoundArray()
{
    CheckValidOfCharacterAudio();
    
    return SoundDataArraySet.CharacterAudioArray;
}

TArray<FBossAudioDataStruct*>& UAudioSubsystem::GetBossSoundArray()
{
    CheckValidOfBossAudio();

    return SoundDataArraySet.BossAudioArray;
}

TArray<FMonsterAudioDataStruct*>& UAudioSubsystem::GetMonsterSoundArray()
{
    CheckValidOfMonsterAudio();

    return SoundDataArraySet.MonsterAudioArray;
}

const float UAudioSubsystem::GetEffectVolume() const
{
    return EffectVolume;
}

const float UAudioSubsystem::GetBgmVolume() const
{
    return BgmVolume;
}

void UAudioSubsystem::SetBgmVolume(float VolumeValue)
{
    BgmVolume = FMath::Clamp(VolumeValue, 0.0f, 1.0f);

    if (IsValid(BgmComp))
    {
        BgmComp->SetVolumeMultiplier(BgmVolume);
    }
}

void UAudioSubsystem::SetEffectVolume(float VolumeValue)
{
    EffectVolume = FMath::Clamp(VolumeValue, 0.0f, 1.0f);

    if (IsValid(UIAudioComp))
    {
        UIAudioComp->SetVolumeMultiplier(BgmVolume);
    }
}

bool UAudioSubsystem::CheckValidOfBgmSource(ELevelSoundType SoundType)
{
    if (BgmSoundMap.Contains(SoundType))
    {
        return true;
    }

    if (!CheckValidOfBgmAudio())
    {
        return false;
    }

    USoundBase* SoundSource = nullptr;

    for (FLevelAudioDataStruct* SoundData : SoundDataArraySet.BgmAudioArray)
    {
        if (SoundData->LevelSoundType == SoundType)
        {
            SoundSource = SoundData->Sound.LoadSynchronous();
            break;
        }
    }
    
    if (!IsValid(SoundSource))
    {
        return false;
    }

    BgmSoundMap.Add(SoundType, SoundSource);

    return true;
}

bool UAudioSubsystem::CheckValidOfUISoundSource(EUISfxSoundType SoundType)
{
    if (UISoundMap.Contains(SoundType))
    {
        return true;
    }

    if (!CheckValidOfUIAudio())
    {
        return false;
    }

    USoundBase* SoundSource = nullptr;

    for (FUISfxAudioDataStruct* SoundData : SoundDataArraySet.UIAudioArray)
    {
        if (SoundData->UISfxSoundType == SoundType)
        {
            SoundSource = SoundData->Sound.LoadSynchronous();
            break;
        }
    }

    if (!IsValid(SoundSource))
    {
        return false;
    }

    UISoundMap.Add(SoundType, SoundSource);

    return true;
}

bool UAudioSubsystem::CheckValidOfBgmAudio()
{
    if (SoundDataArraySet.BgmAudioArray.IsEmpty())
    {
        if (!IsValid(AudioDataSettings))
        {
            return false;
        }

        if (AudioDataSettings->LevelSounds.IsNull())
        {
            return false;
        }

        const FString DataContext(TEXT("Data ConText"));

        LevelSoundTable = AudioDataSettings->LevelSounds.LoadSynchronous();
        LevelSoundTable->GetAllRows(DataContext, SoundDataArraySet.BgmAudioArray);
    }

    return true;
}

bool UAudioSubsystem::CheckValidOfUIAudio()
{
    if (SoundDataArraySet.UIAudioArray.IsEmpty())
    {
        if (!IsValid(AudioDataSettings))
        {
            return false;
        }

        if (AudioDataSettings->UISfxSounds.IsNull())
        {
            return false;
        }

        const FString DataContext(TEXT("Data ConText"));

        UISfxSoundTable = AudioDataSettings->UISfxSounds.LoadSynchronous();
        UISfxSoundTable->GetAllRows(DataContext, SoundDataArraySet.UIAudioArray);
    }

    return true;
}

bool UAudioSubsystem::CheckValidOfCharacterAudio()
{
    if (SoundDataArraySet.CharacterAudioArray.IsEmpty())
    {
        if (!IsValid(AudioDataSettings))
        {
            return false;
        }

        if (AudioDataSettings->CharacterSounds.IsNull())
        {
            return false;
        }

        const FString DataContext(TEXT("Data ConText"));

        CharacterSoundTable = AudioDataSettings->CharacterSounds.LoadSynchronous();
        CharacterSoundTable->GetAllRows(DataContext, SoundDataArraySet.CharacterAudioArray);
    }

    return true;
}

bool UAudioSubsystem::CheckValidOfBossAudio()
{
    if (SoundDataArraySet.BossAudioArray.IsEmpty())
    {
        if (!IsValid(AudioDataSettings))
        {
            return false;
        }

        if (AudioDataSettings->BossSounds.IsNull())
        {
            return false;
        }

        const FString DataContext(TEXT("Data ConText"));

        BossSoundTable = AudioDataSettings->BossSounds.LoadSynchronous();
        BossSoundTable->GetAllRows(DataContext, SoundDataArraySet.BossAudioArray);
    }

    return true;
}

bool UAudioSubsystem::CheckValidOfMonsterAudio()
{
    if (SoundDataArraySet.MonsterAudioArray.IsEmpty())
    {
        if (!IsValid(AudioDataSettings))
        {
            return false;
        }

        if (AudioDataSettings->MonsterSounds.IsNull())
        {
            return false;
        }

        const FString DataContext(TEXT("Data ConText"));

        MonsterSoundTable = AudioDataSettings->MonsterSounds.LoadSynchronous();
        MonsterSoundTable->GetAllRows(DataContext, SoundDataArraySet.MonsterAudioArray);
    }

    return true;
}