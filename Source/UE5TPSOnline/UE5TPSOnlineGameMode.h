// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UE5TPSOnlineGameMode.generated.h"

UCLASS(minimalapi)
class AUE5TPSOnlineGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUE5TPSOnlineGameMode(); 

    virtual void BeginPlay() override;
	
	void StartMatchCountdown();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match Settings")
    float MatchDuration = 300.0f;

    FTimerHandle MatchCountdownTimerHandle;

    void OnMatchCountdownComplete();
};



