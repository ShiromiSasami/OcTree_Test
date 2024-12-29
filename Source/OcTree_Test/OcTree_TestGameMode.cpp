// Copyright Epic Games, Inc. All Rights Reserved.

#include "OcTree_TestGameMode.h"
#include "OcTree_TestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOcTree_TestGameMode::AOcTree_TestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
