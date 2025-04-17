#pragma once

#include "CoreMinimal.h"
#include "OriginalSinPrj/Interface/BattleEvent.h"
#include "GameState/BaseGameState.h"
#include "SingleGameState.generated.h"

UCLASS()
class ORIGINALSINPRJ_API ASingleGameState : public ABaseGameState, public IBattleEvent
{
	GENERATED_BODY()
	
public:

	virtual void ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation) override;
	virtual void TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation) override;
	virtual void OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation) override;
	virtual void OnDeathMonster(AActor* Monster, const FVector& DeathLocation) override;

private:

	void PlayAttackPlayerAnimation();
	void PlayBeAttackedPlayerAnimation(float Damage);
	void PlayAttackMonsterAnimation();
	void PlayBeAttackedMonsterAnimation(float Damage);
};
