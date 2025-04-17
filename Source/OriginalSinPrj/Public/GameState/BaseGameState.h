// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "OriginalSinPrj/GameInstance/OriginalSinPrjGameInstance.h"
#include "OriginalSinPrj/GameInstance/EnumSet.h"
#include "BaseGameState.generated.h"

class UAudioSubsystem;
struct FCharacterAudioDataStruct;
struct FBossAudioDataStruct;
struct FMonsterAudioDataStruct;

UCLASS()
class ORIGINALSINPRJ_API ABaseGameState : public AGameState
{
	GENERATED_BODY()
	
public:
    UFUNCTION(NetMulticast, Unreliable)
    void PlayCharacterSound(UAudioComponent* AudioComp, ECharacterSoundType SoundType);

    UFUNCTION(NetMulticast, Unreliable)
    void PlayBossSound(UAudioComponent* AudioComp, EBossSoundType SoundType);

    UFUNCTION(NetMulticast, Unreliable)
    void PlayMonsterSound(UAudioComponent* AudioComp, EMonsterSoundType SoundType);

    UFUNCTION(NetMulticast, Unreliable)
    void StopEffectSound(UAudioComponent* AudioComp);

protected:
    UFUNCTION(NetMulticast, Reliable)
    void InitCharacterSounds();

    UFUNCTION(NetMulticast, Reliable)
    void InitBossSounds();

    UFUNCTION(NetMulticast, Reliable)
    void InitMonsterSounds();

    virtual void BeginPlay() override;

    void PlaySound(UAudioComponent* AudioComp, USoundBase* SoundSource);

    bool LoadCharacterSoundSourceFromArray(ECharacterSoundType SoundType);
    bool LoadBossSoundSourceFromArray(EBossSoundType SoundType);
    bool LoadMonsterSoundSourceFromArray(EMonsterSoundType SoundType);
    bool CheckValidOfAudioHandle();

protected:
    UPROPERTY()
    TMap<ECharacterSoundType, USoundBase*> CharacterSoundMap;

    UPROPERTY()
    TMap<EBossSoundType, USoundBase*> BossSoundMap;

    UPROPERTY()
    TMap<EMonsterSoundType, USoundBase*> MonsterSoundMap;


    TArray<FCharacterAudioDataStruct*> CharacterSounds;
    TArray<FBossAudioDataStruct*> BossSounds;
    TArray<FMonsterAudioDataStruct*> MonsterSounds;

    UPROPERTY()
    TObjectPtr<UAudioSubsystem> AudioHandle = nullptr;
};
