#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../EnumSet.h"
#include "BuffDataStruct.generated.h"

USTRUCT(BlueprintType)
struct ORIGINALSINPRJ_API FBuffDataStruct : public FTableRowBase
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EBuffType BuffType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;
};