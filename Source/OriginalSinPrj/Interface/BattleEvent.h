#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BattleEvent.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UBattleEvent : public UInterface
{
	GENERATED_BODY()
};

class ORIGINALSINPRJ_API IBattleEvent
{
	GENERATED_BODY()

public:

	UFUNCTION(Category = "Battle Event")
	virtual void ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation) = 0;

	UFUNCTION(Category = "Battle Event")
	virtual void TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation) = 0;

	UFUNCTION(Category = "Battle Event")
	virtual void OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation) = 0;

	UFUNCTION(Category = "Battle Event")
	virtual void OnDeathMonster(AActor* Monster, const FVector& DeathLocation) = 0;
};
