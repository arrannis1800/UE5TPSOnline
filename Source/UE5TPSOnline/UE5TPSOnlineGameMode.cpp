// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5TPSOnlineGameMode.h"
#include "UE5TPSOnlineCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"
#include "TimerManager.h"

AUE5TPSOnlineGameMode::AUE5TPSOnlineGameMode()
{

    bReplicates = true;
    bAlwaysRelevant = true;
}

void AUE5TPSOnlineGameMode::BeginPlay()
{
    Super::BeginPlay();

    StartMatchCountdown();
}

void AUE5TPSOnlineGameMode::StartMatchCountdown()
{
    GetWorldTimerManager().SetTimer(MatchCountdownTimerHandle, this, &AUE5TPSOnlineGameMode::OnMatchCountdownComplete, MatchDuration);
}

void AUE5TPSOnlineGameMode::OnMatchCountdownComplete()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PlayerController = It->Get();
        if (PlayerController)
        {
            AUE5TPSOnlineCharacter* Character = Cast<AUE5TPSOnlineCharacter>(PlayerController->GetPawn());
            if (Character)
            {
                PlayerController->ChangeState(NAME_Spectating);
                PlayerController->ClientGotoState(NAME_Spectating);
            }
        }
    }
}
