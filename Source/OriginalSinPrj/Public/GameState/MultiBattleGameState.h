#pragma once

#include "CoreMinimal.h"
#include "OriginalSinPrj/Interface/BattleEvent.h"
#include "OriginalSinPrj/Interface/MatchManage.h"
#include "GameFramework/GameState.h"
#include "MultiBattleGameState.generated.h"

UCLASS()
class ORIGINALSINPRJ_API AMultiBattleGameState : public AGameState, public IBattleEvent, public IMatchManage
{
	GENERATED_BODY()
	
public:
	virtual void ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation);
	virtual void TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation);
	virtual void OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation);
	virtual void OnDeathMonster(AActor* Monster, const FVector& DeathLocation);
	virtual void StartMatch();
	virtual void FinishMatch();
	virtual void VictoryMatch();
	virtual void DefeatMatch();
	virtual void DrawMatch();

private:

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAttackPlayerAnimation(AActor* Attacker);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayBeAttackedPlayerAnimation(AActor* Attacker);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAttackMonsterAnimation(AActor* Attacker);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayBeAttackedMonsterAnimation(AActor* Attacker);
};