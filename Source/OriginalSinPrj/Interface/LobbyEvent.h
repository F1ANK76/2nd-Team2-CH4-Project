#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LobbyEvent.generated.h"

UINTERFACE(MinimalAPI)
class ULobbyEvent : public UInterface
{
	GENERATED_BODY()
};

class ORIGINALSINPRJ_API ILobbyEvent
{
	GENERATED_BODY()

public:

	UFUNCTION(Category = "Lobby Event")
	virtual void ReadyPlayer(APlayerController* Player) = 0;

	UFUNCTION(Category = "Lobby Event")
	virtual void UnreadyPlayer(APlayerController* Player) = 0;

	UFUNCTION(Category = "Lobby Event")
	virtual void SelectCharacter(const FName& CharacterName, APlayerController* Player) = 0;

	UFUNCTION(Category = "Lobby Event")
	virtual void SelectMap(const FName& MapName) = 0;

	UFUNCTION(Category = "Lobby Event")
	virtual void TravelLevel(const FName& LevelName) = 0;
};
