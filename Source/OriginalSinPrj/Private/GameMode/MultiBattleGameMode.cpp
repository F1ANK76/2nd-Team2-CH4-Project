#include "GameMode/MultiBattleGameMode.h"
#include "GameState/MultiBattleGameState.h"

#include "Kismet/GameplayStatics.h"

void AMultiBattleGameMode::ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation)
{
	/*
	범위 판정->TakeDamage함수로 HP를 감소시켜야하는 액터를 선정
	*/

	if (AMultiBattleGameState* MultiBattleGameState = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		//MultiBattleGameState->ApplyDamage(); -> 공격을 실행한 액터가 공격 애니메이션을 실행하도록 게임스테이트에서 명령
		//TakeDamage(); -> 공격 범위에 있던 액터의 HP를 감소시키고 피격 애니메이션을 실행하도록 명령
	}
}

void AMultiBattleGameMode::TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation)
{
	if (AMultiBattleGameState* MultiBattleGameState = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		MultiBattleGameState->TakeDamage(Victim, Damage, HitLocation);
	}
}

void AMultiBattleGameMode::OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation)
{
	if (AMultiBattleGameState* MultiBattleGameState = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		MultiBattleGameState->OnDeathPlayer(Player, DeathLocation);
	}
}

void AMultiBattleGameMode::OnDeathMonster(AActor* Monster, const FVector& DeathLocation)
{

	if (AMultiBattleGameState* MultiBattleGameState = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		MultiBattleGameState->OnDeathMonster(Monster, DeathLocation);
	}
}

void AMultiBattleGameMode::StartMatch()
{
	if (AMultiBattleGameState* MultiBattleGameState = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		MultiBattleGameState->StartMatch();
	}
}

void AMultiBattleGameMode::FinishMatch()
{
	if (AMultiBattleGameState* MultiBattleGameState = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		MultiBattleGameState->FinishMatch();
	}
}

void AMultiBattleGameMode::VictoryMatch()
{
	if (AMultiBattleGameState* MultiBattleGameState = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		MultiBattleGameState->VictoryMatch();
	}
}

void AMultiBattleGameMode::DefeatMatch()
{
	if (AMultiBattleGameState* MultiBattleGameState = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		MultiBattleGameState->DefeatMatch();
	}
}

void AMultiBattleGameMode::DrawMatch()
{
	if (AMultiBattleGameState* MultiBattleGameState = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		MultiBattleGameState->DrawMatch();
	}
}
