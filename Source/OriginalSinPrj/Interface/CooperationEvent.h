#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CooperationEvent.generated.h"

UINTERFACE(MinimalAPI)
class UCooperationEvent : public UInterface
{
	GENERATED_BODY()
};

class ORIGINALSINPRJ_API ICooperationEvent
{
	GENERATED_BODY()

public:

	UFUNCTION(Category = "Cooperation Event")
	virtual void TravelStageLevel(const FName& NextLevelName) = 0;

	UFUNCTION(Category = "Cooperation Event")
	virtual void AddLifePoint(int LifePointNum) = 0;

	UFUNCTION(Category = "Cooperation Event")
	virtual void SubLifePoint(int LifePointNum) = 0;
};
