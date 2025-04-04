#include "GameMode/MultiBattleGameMode.h"
#include "GameState/MultiBattleGameState.h"

#include "Kismet/GameplayStatics.h"

void AMultiBattleGameMode::ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation)
{
	/*
	���� ����->TakeDamage�Լ��� HP�� ���ҽ��Ѿ��ϴ� ���͸� ����
	*/

	if (AMultiBattleGameState* MultiBattleGameState = Cast<AMultiBattleGameState>(UGameplayStatics::GetGameState(this)))
	{
		//MultiBattleGameState->ApplyDamage(); -> ������ ������ ���Ͱ� ���� �ִϸ��̼��� �����ϵ��� ���ӽ�����Ʈ���� ���
		//TakeDamage(); -> ���� ������ �ִ� ������ HP�� ���ҽ�Ű�� �ǰ� �ִϸ��̼��� �����ϵ��� ���
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
