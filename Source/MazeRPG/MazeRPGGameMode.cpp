// Copyright Epic Games, Inc. All Rights Reserved.

#include "MazeRPGGameMode.h"
#include "MazeRPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMazeRPGGameMode::AMazeRPGGameMode()
{
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_SparrowCharacter"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }

}
