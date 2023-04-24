// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThirdPersonCPPGameMode.h"
#include "ThirdPersonCPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThirdPersonCPPGameMode::AThirdPersonCPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
