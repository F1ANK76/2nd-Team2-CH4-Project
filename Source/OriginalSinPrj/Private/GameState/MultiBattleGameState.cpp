#include "GameState/MultiBattleGameState.h"

void AMultiBattleGameState::ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation)
{
}

void AMultiBattleGameState::TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation)
{
}

void AMultiBattleGameState::OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation)
{
}

void AMultiBattleGameState::OnDeathMonster(AActor* Monster, const FVector& DeathLocation)
{
}

void AMultiBattleGameState::StartMatch()
{
}

void AMultiBattleGameState::FinishMatch()
{
}

void AMultiBattleGameState::VictoryMatch()
{
}

void AMultiBattleGameState::DefeatMatch()
{
}

void AMultiBattleGameState::DrawMatch()
{
}

void AMultiBattleGameState::Multicast_ApplyDamage_Implementation(AActor* Attacker, float Damage, const FVector& HitLocation)
{
}

void AMultiBattleGameState::Multicast_TakeDamage_Implementation(AActor* Victim, float Damage, const FVector& HitLocation)
{
}

void AMultiBattleGameState::Multicast_PlayAttackPlayerAnimation_Implementation(AActor* Attacker)
{
}

void AMultiBattleGameState::Multicast_PlayBeAttackedPlayerAnimation_Implementation(AActor* Attacker)
{
}

void AMultiBattleGameState::Multicast_PlayAttackMonsterAnimation_Implementation(AActor* Attacker)
{
}

void AMultiBattleGameState::Multicast_PlayBeAttackedMonsterAnimation_Implementation(AActor* Attacker)
{
}
