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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int ObstaclesCompleted;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int TotalObstacles;

	TArray<AActor*> ActorsObstaclesFound;

	virtual void BeginPlay() override;

public:
	ABlueGravityGameMode();
	void AddGlobalPoints(float Value);
	void GetTotalObstacles();
	void AddGlobalObstacleCompleted();
};