// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnumSet.h"
#include "Kismet/GameplayStatics.h"
#include "AudioSubsystem.generated.h"

class UAudioDataSettings;

UCLASS()
class ORIGINALSINPRJ_API UAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void LoadDataTables();
	void CheckLoadedLevelSound();

	void PlayBGMByLevelType(ELevelType LevelType);
	void PlayBGM(ELevelSoundType SoundType);
	void PlaySFX(ESfxSoundType SoundType, uint8 DetailSoundType, FVector Location = FVector::ZeroVector);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetAndApplyMasterVolume(float NewVolume);

	template<typename EnumType, typename StructType>
	void PlaySFXByType(UObject* WorldContext, UDataTable* Table, uint8 DetailSoundType, FVector Location);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float MasterVolume = 1.0f;

	UPROPERTY()
	UDataTable* LevelSoundTable;

	UPROPERTY()
	UDataTable* UISfxSoundTable;

	UPROPERTY()
	UDataTable* MonsterSoundTable;

	UPROPERTY()
	UDataTable* CharacterSoundTable;

	UPROPERTY()
	UDataTable* BossSoundTable;

	UPROPERTY()
	TObjectPtr<UAudioComponent> BgmComp;

	const UAudioDataSettings* AudioDataSettings;
};

// Use EX : PlaySFX(ESfxSoundType::Monster, static_cast<uint8>(EMonsterSoundType::Attack), FVector(10.f, 0.f, 0.f));
template<typename EnumType, typename StructType>
inline void UAudioSubsystem::PlaySFXByType(UObject* WorldContext, UDataTable* Table, uint8 DetailSoundType, FVector Location)
{
	UEnum* EnumPtr = StaticEnum<EnumType>();

	if (Table && EnumPtr)
	{
		FName RowName = FName(*EnumPtr->GetNameStringByValue(static_cast<int64>(DetailSoundType)));
		const StructType* FoundRow = Table->FindRow<StructType>(RowName, TEXT("PlaySFXByType"));

		if (FoundRow && !FoundRow->Sound.IsNull())
		{
			USoundBase* Sound = FoundRow->Sound.LoadSynchronous();

			if (Sound && WorldContext)
			{
				if (Location.IsZero())
				{
					// UI
					UGameplayStatics::PlaySound2D(WorldContext, Sound, MasterVolume);
					UE_LOG(LogTemp, Warning, TEXT("Play 2D Sound"));
				}
				else
				{
					UGameplayStatics::PlaySoundAtLocation(WorldContext, Sound, Location, MasterVolume);
					UE_LOG(LogTemp, Warning, TEXT("Play 3D Sound"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Sound or WorldContext is NULL"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FoundRow is NULL"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SFX Table or EnumPtr is NULL"));
	}
}
