#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Player/WitchTypes.h"
#include "CharacterDataStruct.generated.h"

class ABaseWitch;

USTRUCT(BlueprintType)
struct ORIGINALSINPRJ_API FCharacterDataStruct : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName CharacterName = "";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECharacterType CharacterType = ECharacterType::Hand;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UTexture2D> PortraitImage = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ABaseWitch> WitchClass = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ABaseWitch> AIWitchClass = nullptr;
};
