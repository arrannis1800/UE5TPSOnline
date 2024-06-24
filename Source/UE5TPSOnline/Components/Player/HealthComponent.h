// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5TPSONLINE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetCurrentHealth(float& MaxHealth);

    UFUNCTION(BlueprintCallable, Category = "Health")
    void AddHealth(float AddHealth);

    UFUNCTION(BlueprintCallable, Category = "Health")
    float TakeDamage(float Damage);

protected:
    UFUNCTION()
    void OnRep_Health();

    UPROPERTY(EditDefaultsOnly, Category = "Health")
    float MaxHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Health")
    float Regeneration = 5.0f;

    UPROPERTY(Replicated, ReplicatedUsing = "OnRep_Health")
    float Health = MaxHealth;

    UPROPERTY(Replicated)
    bool bIsAlive = true;

    void OnHealthUpdate();

    UPROPERTY(EditDefaultsOnly)
    UAnimSequence* DeathAnimmation;
};
