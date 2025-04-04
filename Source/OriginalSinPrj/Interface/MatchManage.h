#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MatchManage.generated.h"

UINTERFACE(MinimalAPI)
class UMatchManage : public UInterface
{
	GENERATED_BODY()
};

class ORIGINALSINPRJ_API IMatchManage
{
	GENERATED_BODY()

public:

	UFUNCTION(Category = "Match Manage")
	virtual void StartMatch() = 0;

	UFUNCTION(Category = "Match Manage")
	virtual void FinishMatch() = 0;

	UFUNCTION(Category = "Match Manage")
	virtual void VictoryMatch() = 0;

	UFUNCTION(Category = "Match Manage")
	virtual void DefeatMatch() = 0;

	UFUNCTION(Category = "Match Manage")
	virtual void DrawMatch() = 0;
};
