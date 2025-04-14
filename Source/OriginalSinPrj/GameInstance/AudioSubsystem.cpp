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
    // ��ġ�� ���� �Ҹ� ��� �ʿ�, UI ���常 ó�� ����
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
        // ���� ���� 0���� �ϸ� BGM ������ ���缭 �Ͻ������� �ӽ� ��ġ
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