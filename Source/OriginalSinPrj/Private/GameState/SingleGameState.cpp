#include "GameState/SingleGameState.h"

void ASingleGameState::ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation)
{
}

void ASingleGameState::TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation)
{
}

void ASingleGameState::OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation)
{
}

void ASingleGameState::OnDeathMonster(AActor* Monster, const FVector& DeathLocation)
{
}

void ASingleGameState::PlayAttackPlayerAnimation()
{
}

void ASingleGameState::PlayBeAttackedPlayerAnimation(float Damage)
{
}

void ASingleGameState::PlayAttackMonsterAnimation()
{
}

void ASingleGameState::PlayBeAttackedMonsterAnimation(float Damage)
{
}
