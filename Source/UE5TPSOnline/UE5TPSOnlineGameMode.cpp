// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5TPSOnlineGameMode.h"
#include "UE5TPSOnlineCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE5TPSOnlineGameMode::AUE5TPSOnlineGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Players/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
