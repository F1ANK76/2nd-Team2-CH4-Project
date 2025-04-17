// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/BaseGameState.h"
#include "OriginalSinPrj/GameInstance/AudioSubsystem.h"
#include "OriginalSinPrj/GameInstance/Struct/CharacterAudioDataStruct.h"
#include "OriginalSinPrj/GameInstance/Struct/BossAudioDataStruct.h"
#include "OriginalSinPrj/GameInstance/Struct/MonsterAudioDataStruct.h"
#include "Components/AudioComponent.h"


void ABaseGameState::BeginPlay()
{
	Super::BeginPlay();

	InitCharacterSounds();
	InitBossSounds();
	InitMonsterSounds();
}

void ABaseGameState::PlaySound(UAudioComponent* AudioComp, USoundBase* SoundSource)
{
	if (!IsValid(AudioComp))
	{
		return;
	}

	if (!IsValid(SoundSource))
	{
		return;
	}

	if (!CheckValidOfAudioHandle())
	{
		return;
	}

	float Volume = AudioHandle->GetEffectVolume();

	if (AudioComp->IsPlaying())
	{
		AudioComp->Stop();
	}

	AudioComp->SetVolumeMultiplier(Volume);
	AudioComp->SetSound(SoundSource);
	AudioComp->Play();
}

void ABaseGameState::StopEffectSound_Implementation(UAudioComponent* AudioComp)
{
	if (!IsValid(AudioComp))
	{
		return;
	}

	if (AudioComp->IsPlaying())
	{
		AudioComp->Stop();
	}
}

void ABaseGameState::PlayCharacterSound_Implementation(UAudioComponent* AudioComp, ECharacterSoundType SoundType)
{
	if (!CharacterSoundMap.Contains(SoundType))
	{
		if (!LoadCharacterSoundSourceFromArray(SoundType))
		{
			return;
		}
	}

	USoundBase* SoundSource = CharacterSoundMap[SoundType];

	PlaySound(AudioComp, SoundSource);
}

void ABaseGameState::PlayBossSound_Implementation(UAudioComponent* AudioComp, EBossSoundType SoundType)
{
	if (!BossSoundMap.Contains(SoundType))
	{
		if (!LoadBossSoundSourceFromArray(SoundType))
		{
			return;
		}
	}

	USoundBase* SoundSource = BossSoundMap[SoundType];

	PlaySound(AudioComp, SoundSource);
}

void ABaseGameState::PlayMonsterSound_Implementation(UAudioComponent* AudioComp, EMonsterSoundType SoundType)
{
	if (!MonsterSoundMap.Contains(SoundType))
	{
		if (!LoadMonsterSoundSourceFromArray(SoundType))
		{
			return;
		}
	}

	USoundBase* SoundSource = MonsterSoundMap[SoundType];

	PlaySound(AudioComp, SoundSource);
}

void ABaseGameState::InitCharacterSounds_Implementation()
{
	if (!CheckValidOfAudioHandle())
	{
		return;
	}

	CharacterSounds = AudioHandle->GetCharacterSoundArray();
}

void ABaseGameState::InitBossSounds_Implementation()
{
	if (!CheckValidOfAudioHandle())
	{
		return;
	}

	BossSounds = AudioHandle->GetBossSoundArray();
}

void ABaseGameState::InitMonsterSounds_Implementation()
{
	if (!CheckValidOfAudioHandle())
	{
		return;
	}

	MonsterSounds = AudioHandle->GetMonsterSoundArray();
}

bool ABaseGameState::LoadCharacterSoundSourceFromArray(ECharacterSoundType SoundType)
{
	if (CharacterSounds.IsEmpty())
	{
		return false;
	}

	USoundBase* SoundSource = nullptr;

	for (FCharacterAudioDataStruct* SoundData : CharacterSounds)
	{
		if (SoundType == SoundData->CharacterSoundType)
		{
			SoundSource = SoundData->Sound.LoadSynchronous();
			break;
		}
	}

	if (!IsValid(SoundSource))
	{
		return false;
	}

	CharacterSoundMap.Add(SoundType, SoundSource);

	return true;
}

bool ABaseGameState::LoadBossSoundSourceFromArray(EBossSoundType SoundType)
{
	if (BossSounds.IsEmpty())
	{
		return false;
	}

	USoundBase* SoundSource = nullptr;

	for (FBossAudioDataStruct* SoundData : BossSounds)
	{
		if (SoundType == SoundData->BossSoundType)
		{
			SoundSource = SoundData->Sound.LoadSynchronous();
			break;
		}
	}

	if (!IsValid(SoundSource))
	{
		return false;
	}

	BossSoundMap.Add(SoundType, SoundSource);

	return true;
}

bool ABaseGameState::LoadMonsterSoundSourceFromArray(EMonsterSoundType SoundType)
{
	if (MonsterSounds.IsEmpty())
	{
		return false;
	}

	USoundBase* SoundSource = nullptr;

	for (FMonsterAudioDataStruct* SoundData : MonsterSounds)
	{
		if (SoundType == SoundData->MonsterSoundType)
		{
			SoundSource = SoundData->Sound.LoadSynchronous();
			break;
		}
	}

	if (!IsValid(SoundSource))
	{
		return false;
	}

	MonsterSoundMap.Add(SoundType, SoundSource);

	return true;
}

bool ABaseGameState::CheckValidOfAudioHandle()
{
	if (IsValid(AudioHandle))
	{
		return true;
	}

	if (!IsValid(GetGameInstance()))
	{
		return false;
	}

	AudioHandle = GetGameInstance()->GetSubsystem<UAudioSubsystem>();

	if (!IsValid(AudioHandle))
	{
		return false;
	}

	return true;
}