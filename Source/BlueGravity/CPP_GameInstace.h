// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CPP_GameInstace.generated.h"

//Storage the global variables for other levels
UCLASS()
class BLUEGRAVITY_API UCPP_GameInstace : public UGameInstance
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int Instance_Obstacles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int Instance_Time;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Instance_Score;
};