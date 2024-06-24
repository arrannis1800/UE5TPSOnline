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
	DOREPLIFETIME(UHealthComponent, bIsAlive);
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsAlive)
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
	if (GetOwner()->HasAuthority())
	{
		Health -= Damage;

		Health = FMath::Clamp(Health, 0.0f, MaxHealth);

		OnRep_Health();
		if (Health <= 0.0f)
		{
			bIsAlive = false;
			UE_LOG(LogTemp, Warning, TEXT("IsAlive has changed to: %d"), bIsAlive);
			OnRep_Health();
		}
	}

	return Health;
}

void UHealthComponent::OnRep_Health()
{
	if (DeathAnimmation)
	{
		Cast<ACharacter>(GetOwner())->GetMesh()->PlayAnimation(DeathAnimmation, false);
	}
	OnHealthUpdate();
}
void UHealthComponent::OnHealthUpdate()
{
}

