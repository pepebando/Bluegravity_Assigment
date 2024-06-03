// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlueGravityGameMode.h"
#include "BlueGravityCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "CPP_GameInstace.h"
#include "CPP_ObsBase.h"

ABlueGravityGameMode::ABlueGravityGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}


}

void ABlueGravityGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetTotalObstacles();
	Time = 120;
	GameOver = false;
}

//Initialize timer
void ABlueGravityGameMode::GameStart() {
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABlueGravityGameMode::TimerFunction, 1.0f, true);
}

//Add Score
void ABlueGravityGameMode::AddGlobalPoints(float Value) {
	GlobalScore += Value;
}

//Add Obstacles passed
void ABlueGravityGameMode::AddGlobalObstacleCompleted() {
	ObstaclesCompleted ++;
}

//Reduce time until it reach 0
void ABlueGravityGameMode::TimerFunction()
{
	if (GameOver == true) {
		SendVariablesToInstance();
		UGameplayStatics::OpenLevel(this, FName("/Game/GameOver"), true);

		return;
	}
	Time--;
}

void ABlueGravityGameMode::SendVariablesToInstance()
{
	UCPP_GameInstace* GameInstance = Cast<UCPP_GameInstace>(GetGameInstance());

	if (GameInstance)
	{
		// Access and modify variables
		GameInstance->Instance_Obstacles = ObstaclesCompleted;
		GameInstance->Instance_Score = GlobalScore;
		GameInstance->Instance_Time = Time;
	}
}


//Get All Obstacles in map
void ABlueGravityGameMode::GetTotalObstacles()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPP_ObsBase::StaticClass(), ActorsObstaclesFound);
	TotalObstacles= ActorsObstaclesFound.Num();
}
