// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlueGravityGameMode.h"
#include "BlueGravityCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
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

}

void ABlueGravityGameMode::AddGlobalPoints(float Value) {
	GlobalScore += Value;
}

void ABlueGravityGameMode::AddGlobalObstacleCompleted() {
	ObstaclesCompleted ++;
}

void ABlueGravityGameMode::GetTotalObstacles()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPP_ObsBase::StaticClass(), ActorsObstaclesFound);
	TotalObstacles= ActorsObstaclesFound.Num();
}
