// AudioSubsystem.cpp

#include "AudioSubsystem.h"
#include "AudioDataSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Struct/LevelAudioDataStruct.h"
#include "Struct/MonsterAudioDataStruct.h"
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
        if (!AudioDataSettings->LevelSounds.IsNull())
        {
            LevelSoundTable = AudioDataSettings->LevelSounds.LoadSynchronous();
        }

        if (!AudioDataSettings->MonsterSounds.IsNull())
        {
            MonsterSoundTable = AudioDataSettings->MonsterSounds.LoadSynchronous();
        }
    }
}

void UAudioSubsystem::PlayBGM(ELevelSoundType SoundType)
{
    if (GetWorld())
    {
        UDataTable* TargetTable = LevelSoundTable;
        UEnum* EnumPtr = StaticEnum<ELevelSoundType>();

        if (TargetTable)
        {
            FName RowName = FName(*EnumPtr->GetNameStringByValue(static_cast<int64>(SoundType)));

            const FLevelAudioDataStruct* FoundRow = TargetTable->FindRow<FLevelAudioDataStruct>(RowName, TEXT("PlayBGM AudioSubsystem"));

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
                            UE_LOG(LogTemp, Warning, TEXT("Stop Sound"));
                        }

                        BgmComp = UGameplayStatics::CreateSound2D(GetWorld(), Sound, MasterVolume);

                        if (BgmComp)
                        {
                            BgmComp->Play();
                            UE_LOG(LogTemp, Warning, TEXT("Play Sound"));
                        }
                    }
                }
            }
        }
    }
}

void UAudioSubsystem::PlaySFX(ESfxSoundType SoundType, uint8 DetailSoundType, FVector Location)
{
    // 위치에 따른 소리 재생 필요, UI 사운드만 처리 가능
    switch (SoundType)
    {
        case ESfxSoundType::Monster:
            PlaySFXByType<EMonsterSoundType, FMonsterAudioDataStruct>(GetWorld(), MonsterSoundTable, DetailSoundType, Location);
            break;

        default:
            UE_LOG(LogTemp, Warning, TEXT("Invalid SFX type"));
            return;
    }
}

// Only BGM
void UAudioSubsystem::SetAndApplyMasterVolume(float NewVolume)
{
    MasterVolume = FMath::Clamp(NewVolume, 0.0f, 1.0f);

    if (BgmComp)
    {
        // 현재 볼륨 0으로 하면 BGM 완전히 멈춰서 일시정지로 임시 조치
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