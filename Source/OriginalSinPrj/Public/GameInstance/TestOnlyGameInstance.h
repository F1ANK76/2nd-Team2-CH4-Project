#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OriginalSinPrj/Interface/SessionManage.h"
#include "TestOnlyGameInstance.generated.h"

class UMyOnlineSubsystem;

UCLASS()
class ORIGINALSINPRJ_API UTestOnlyGameInstance : public UGameInstance, public ISessionManage
{
	GENERATED_BODY()

public:
	virtual void Init();

	virtual void CreateGameSession(const FString& SessionName) override;
	virtual void JoinGameSession(const FOnlineSessionSearchResult& SearchResult) override;
	virtual void FindGameSessions() override;
	virtual void DestroyGameSession() override;

private:
	UMyOnlineSubsystem* OnlineSubsystem;
};