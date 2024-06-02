// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BlueGravityGameMode.generated.h"

UCLASS(minimalapi)
class ABlueGravityGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float GlobalScore;

public:
	ABlueGravityGameMode();
	void AddGlobalPoints(float Value);

};



