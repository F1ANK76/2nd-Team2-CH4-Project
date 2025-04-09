#include "SingleGameMode.h"
#include "GameState/SingleGameState.h"

#include "Kismet/GameplayStatics.h"

void ASingleGameMode::ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation)
{
	if (ASingleGameState* SingleGameState = Cast<ASingleGameState>(UGameplayStatics::GetGameState(this)))
	{
		SingleGameState->ApplyDamage(Attacker, Damage, HitLocation);
	}
}

void ASingleGameMode::TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation)
{
	if (ASingleGameState* SingleGameState = Cast<ASingleGameState>(UGameplayStatics::GetGameState(this)))
	{
		SingleGameState->TakeDamage(Victim, Damage, HitLocation);
	}
}

void ASingleGameMode::OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation)
{
	if (ASingleGameState* SingleGameState = Cast<ASingleGameState>(UGameplayStatics::GetGameState(this)))
	{
		SingleGameState->OnDeathPlayer(Player, DeathLocation);
	}
}

void ASingleGameMode::OnDeathMonster(AActor* Monster, const FVector& DeathLocation)
{
	if (ASingleGameState* SingleGameState = Cast<ASingleGameState>(UGameplayStatics::GetGameState(this)))
	{
		SingleGameState->OnDeathMonster(Monster, DeathLocation);
	}
}
