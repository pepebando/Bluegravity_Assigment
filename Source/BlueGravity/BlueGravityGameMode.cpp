// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlueGravityGameMode.h"
#include "BlueGravityCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABlueGravityGameMode::ABlueGravityGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}