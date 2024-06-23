// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	// ...
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, Health);
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsAlive && Health < MaxHealth)
	{
		AddHealth(DeltaTime * Regeneration);
	}
}

float UHealthComponent::GetCurrentHealth(float& MaximumHealth)
{
	MaximumHealth = MaxHealth;
	return Health;
}

void UHealthComponent::AddHealth(float AddHealth)
{
	Health += AddHealth;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

float UHealthComponent::TakeDamage(float Damage)
{
	if (GetOwner()->HasAuthority()) // Only execute on the server
	{
		Health -= Damage;

		// Ensure Health is clamped correctly
		Health = FMath::Clamp(Health, 0.0f, MaxHealth);

		// Trigger replication to clients
		OnRep_Health();
	}

	return Health;
}

void UHealthComponent::OnRep_Health()
{
	OnHealthUpdate();
}

void UHealthComponent::OnHealthUpdate()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter && OwnerCharacter->IsLocallyControlled())
	{
		if (Health <= 0)
		{
			bIsAlive = false;
		}
	}
}

