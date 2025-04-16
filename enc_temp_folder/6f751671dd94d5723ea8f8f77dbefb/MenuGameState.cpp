#include "MenuGameState.h"

#include "Kismet/GameplayStatics.h"
#include "OriginalSinPrj/GameInstance/AudioSubsystem.h"

#include "OriginalSinPrj/GameInstance/Struct/CharacterAudioDataStruct.h"
#include "OriginalSinPrj/GameInstance/Struct/BossAudioDataStruct.h"
#include "OriginalSinPrj/GameInstance/Struct/MonsterAudioDataStruct.h"
#include "Components/AudioComponent.h"

/*------------------------����------------------------*/
void AMenuGameState::TravelLevel(const FName& LevelName)
{
	if (!bIsSessionConnected)
	{
		Local_TravelLevel(LevelName);
	}
	else
	{
		Multicast_TravelLevel(LevelName);
	}
}

void AMenuGameState::SelectCharacter(const FName& CharacterName, APlayerController* Player)
{
	if (!bIsSessionConnected)
	{
		Local_SelectCharacter(CharacterName, Player);
	}
	else
	{
		Multicast_SelectCharacter(CharacterName, Player);
	}
}

void AMenuGameState::SelectMap(const FName& MapName)
{
}

/*---------------------�����÷���---------------------*/
void AMenuGameState::Local_TravelLevel(const FName& LevelName)
{
	UGameplayStatics::OpenLevel(this, LevelName);
}

void AMenuGameState::Local_SelectCharacter(const FName& CharacterName, APlayerController* Player)
{

}

/*---------------------��Ƽ�÷���---------------------*/
void AMenuGameState::ReadyPlayer(APlayerController* Player)
{
	Multicast_ReadyPlayer(Player);
}

void AMenuGameState::UnreadyPlayer(APlayerController* Player)
{
	Multicast_UnreadyPlayer(Player);
}

void AMenuGameState::JoinSessionPlayer(const FString& PlayerName)
{
	Multicast_JoinSessionPlayer(PlayerName);
}

void AMenuGameState::ExitSessionPlayer(const FString& PlayerName)
{
	Multicast_ExitSessionPlayer(PlayerName);
}

bool AMenuGameState::GetIsSessionConnected() const
{
	return bIsSessionConnected;
}

void AMenuGameState::SetIsSessionConnected(bool IsSessionConnected)
{
	bIsSessionConnected = IsSessionConnected;
}

void AMenuGameState::CreateGameSession(const FString& SessionName)
{
}

void AMenuGameState::JoinGameSession(const FOnlineSessionSearchResult& SearchResult)
{
}

void AMenuGameState::FindGameSessions()
{
}

void AMenuGameState::DestroyGameSession()
{
}

void AMenuGameState::Multicast_ReadyPlayer_Implementation(APlayerController* Player)
{
	/*if (PlayerList.Contains())
	{
		PlayerList[] = true;
	}*/
}

void AMenuGameState::Multicast_UnreadyPlayer_Implementation(APlayerController* Player)
{
	/*if (PlayerList.Contains())
	{
		PlayerList[] = false;
	}*/
}

void AMenuGameState::Multicast_SelectCharacter_Implementation(const FName& CharacterName, APlayerController* Player)
{
}

void AMenuGameState::Multicast_TravelLevel_Implementation(const FName& LevelName)
{
}

void AMenuGameState::Multicast_JoinSessionPlayer_Implementation(const FString& PlayerName)
{
	PlayerList.Add(PlayerName, false);
}

void AMenuGameState::Multicast_ExitSessionPlayer_Implementation(const FString& PlayerName)
{
	PlayerList.Remove(PlayerName);
}

void AMenuGameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		InitCharacterSounds();
		InitBossSounds();
		InitMonsterSounds();
	}
}

void AMenuGameState::InitCharacterSounds_Implementation()
{
	if (!CheckValidOfAudioHandle())
	{
		return;
	}

	CharacterSounds = AudioHandle->GetCharacterSoundArray();
}

void AMenuGameState::InitBossSounds_Implementation()
{
	if (!CheckValidOfAudioHandle())
	{
		return;
	}

	BossSounds = AudioHandle->GetBossSoundArray();
}

void AMenuGameState::InitMonsterSounds_Implementation()
{
	if (!CheckValidOfAudioHandle())
	{
		return;
	}

	MonsterSounds = AudioHandle->GetMonsterSoundArray();
}

void AMenuGameState::PlayCharacterSound_Implementation(UAudioComponent* AudioComp, ECharacterSoundType SoundType)
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

void AMenuGameState::PlayBossSound_Implementation(UAudioComponent* AudioComp, EBossSoundType SoundType)
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

void AMenuGameState::PlayMonsterSound_Implementation(UAudioComponent* AudioComp, EMonsterSoundType SoundType)
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

void AMenuGameState::PlaySound(UAudioComponent* AudioComp, USoundBase* SoundSource)
{
	if (!IsValid(AudioComp))
	{
		UE_LOG(LogTemp, Warning, TEXT("Audio Comp is invalid"));
		return;
	}

	if (!IsValid(SoundSource))
	{
		UE_LOG(LogTemp, Warning, TEXT("Sound Source is invalid"));
		return;
	}

	if (!CheckValidOfAudioHandle())
	{
		UE_LOG(LogTemp, Warning, TEXT("Audio Handle is invalid"));
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

bool AMenuGameState::LoadCharacterSoundSourceFromArray(ECharacterSoundType SoundType)
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

bool AMenuGameState::LoadBossSoundSourceFromArray(EBossSoundType SoundType)
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

bool AMenuGameState::LoadMonsterSoundSourceFromArray(EMonsterSoundType SoundType)
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

bool AMenuGameState::CheckValidOfAudioHandle()
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
