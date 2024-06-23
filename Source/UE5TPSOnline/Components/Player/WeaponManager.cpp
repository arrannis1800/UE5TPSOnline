// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponManager.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h" 

// Sets default values for this component's properties
UWeaponManagerComponent::UWeaponManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	// ...
}

void UWeaponManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UWeaponManagerComponent, CurrentWeapon);
}


// Called when the game starts
void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	SwitchWeapon();

	// ...
	
}


// Called every frame
void UWeaponManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponManagerComponent::SwitchWeapon()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponIndex: %d"), WeaponIndex);
		if (Weapons.Num() == 0)
			return;

		if (Weapons[WeaponIndex])
		{
			if (CurrentWeapon)
				CurrentWeapon->Destroy();

			CurrentWeapon = GetWorld()->SpawnActor<ABaseWeapon>(Weapons[WeaponIndex]);
			if (CurrentWeapon)
			{
				CurrentWeapon->SetDefaultAmmo();
				OnRep_CurrentWeapon();
				// Attach the weapon to the character
			}

			WeaponIndex = (++WeaponIndex) % Weapons.Num();
		}
	}
	else
	{
		ServerSwitchWeapon();
	}
}

void UWeaponManagerComponent::ServerSwitchWeapon_Implementation()
{
	SwitchWeapon();
}

bool UWeaponManagerComponent::ServerSwitchWeapon_Validate()
{
	return true;
}

void UWeaponManagerComponent::Fire()
{
	CurrentWeapon->Fire();
}

void UWeaponManagerComponent::OnRep_CurrentWeapon()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		CurrentWeapon->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
		CurrentWeapon->SetOwner(OwnerCharacter);
	}
}

