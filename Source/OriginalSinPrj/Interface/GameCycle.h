#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameCycle.generated.h"

UINTERFACE(MinimalAPI)
class UGameCycle : public UInterface
{
	GENERATED_BODY()
};

class ORIGINALSINPRJ_API IGameCycle
{
	GENERATED_BODY()

public:

	UFUNCTION(Category = "Game Cycle")
	virtual void StartGame() = 0;

	UFUNCTION(Category = "Game Cycle")
	virtual void FinishGame() = 0;
};
