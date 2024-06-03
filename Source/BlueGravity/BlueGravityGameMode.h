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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int Time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool GameOver;

	TArray<AActor*> ActorsObstaclesFound;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "FunctionLibrary")
	void GameStart();

private: 
	//Timer function for countdown
	FTimerHandle TimerHandle;

public:
	//Functions that are going to be call globally
	ABlueGravityGameMode();
	void AddGlobalPoints(float Value);
	void GetTotalObstacles();
	void AddGlobalObstacleCompleted();
	void TimerFunction();
	void SendVariablesToInstance();
};