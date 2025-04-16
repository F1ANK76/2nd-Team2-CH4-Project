#include "MenuGameState.h"

#include "Kismet/GameplayStatics.h"
#include "OriginalSinPrj/GameInstance/AudioSubsystem.h"

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
	if (!IsValid(GetGameInstance()))
	{
		return;
	}

	UAudioSubsystem* AudioHandle = GetGameInstance()->GetSubsystem<UAudioSubsystem>();

	if (!IsValid(AudioHandle))
	{
		return;
	}

	CharacterSounds = AudioHandle->GetCharacterSoundArray();
}

void AMenuGameState::InitBossSounds_Implementation()
{
	if (!IsValid(GetGameInstance()))
	{
		return;
	}

	UAudioSubsystem* AudioHandle = GetGameInstance()->GetSubsystem<UAudioSubsystem>();

	if (!IsValid(AudioHandle))
	{
		return;
	}

	BossSounds = AudioHandle->GetBossSoundArray();
}

void AMenuGameState::InitMonsterSounds_Implementation()
{
	if (!IsValid(GetGameInstance()))
	{
		return;
	}

	UAudioSubsystem* AudioHandle = GetGameInstance()->GetSubsystem<UAudioSubsystem>();

	if (!IsValid(AudioHandle))
	{
		return;
	}

	MonsterSounds = AudioHandle->GetMonsterSoundArray();
}

void AMenuGameState::PlayCharacterSound_Implementation(UAudioComponent* AudioComp, ECharacterSoundType SoundType)
{

}

void AMenuGameState::PlayBossSound_Implementation(UAudioComponent* AudioComp, EBossSoundType SoundType)
{

}

void AMenuGameState::PlayMonsterSound_Implementation(UAudioComponent* AudioComp, EMonsterSoundType SoundType)
{

}

