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
	AMultiBattleGameState();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation) override;
	virtual void TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation) override;
	virtual void OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation) override;
	virtual void OnDeathMonster(AActor* Monster, const FVector& DeathLocation) override;
	virtual void FinishMatch() override;
	virtual void VictoryMatch() override;
	virtual void DefeatMatch() override;
	virtual void DrawMatch() override;

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