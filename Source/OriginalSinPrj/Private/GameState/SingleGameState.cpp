#include "GameState/SingleGameState.h"
#include "OriginalSinPrj/GameInstance/OriginalSinPrjGameInstance.h"
#include "OriginalSinPrj/GameInstance/UISubsystem.h"

void ASingleGameState::ApplyDamage(AActor* Attacker, float Damage, const FVector& HitLocation)
{
}

void ASingleGameState::TakeDamage(AActor* Victim, float Damage, const FVector& HitLocation)
{
}

void ASingleGameState::OnDeathPlayer(ACharacter* Player, const FVector& DeathLocation)
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        int32 PlayerLife = MyGI->GetPlayerFarmingLevel();

        // UE_LOG(LogTemp, Warning, TEXT("PlayerLife %d"), PlayerLife);

        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            UISubsystem->ShowWidget(EAddWidgetType::ResultWidget);
            UISubsystem->SetMouseMode(true);

            UE_LOG(LogTemp, Warning, TEXT("SingleGameMode OnDeathPlayer"));
        }
    }
}

void ASingleGameState::OnDeathMonster(AActor* Monster, const FVector& DeathLocation)
{
    if (UOriginalSinPrjGameInstance* MyGI = Cast<UOriginalSinPrjGameInstance>(GetWorld()->GetGameInstance()))
    {
        if (UUISubsystem* UISubsystem = MyGI->GetSubsystem<UUISubsystem>())
        {
            UISubsystem->ShowWidget(EAddWidgetType::ResultWidget);
            UISubsystem->SetMouseMode(true);

            UE_LOG(LogTemp, Warning, TEXT("SingleGameMode OnDeathMonster"));
        }
    }
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
