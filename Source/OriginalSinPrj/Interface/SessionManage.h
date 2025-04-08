#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SessionManage.generated.h"

UINTERFACE(MinimalAPI)
class USessionManage : public UInterface
{
	GENERATED_BODY()
};

class ORIGINALSINPRJ_API ISessionManage
{
	GENERATED_BODY()

public:
	UFUNCTION(Category = "SessionManage")
	virtual void CreateGameSession(const FString& SessionName) = 0;

	UFUNCTION(Category = "SessionManage")
	virtual void FindGameSessions() = 0;

	UFUNCTION(Category = "SessionManage")
	virtual void JoinGameSession(const FOnlineSessionSearchResult& SearchResult) = 0;

	UFUNCTION(Category = "SessionManage")
	virtual void DestroyGameSession() = 0;
};
