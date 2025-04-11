#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterDataStruct.generated.h"

USTRUCT(BlueprintType)
struct ORIGINALSINPRJ_API FCharacterDataStruct : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText CharacterName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UTexture2D> PortraitImage;
};
