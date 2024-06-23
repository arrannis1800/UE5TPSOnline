// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UE5TPSOnline/BaseWeapon.h"
#include "WeaponManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5TPSONLINE_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponManagerComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SwitchWeapon();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSwitchWeapon();

	void Fire();

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ABaseWeapon>> Weapons;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	ABaseWeapon* CurrentWeapon;

private:
	int32 WeaponIndex;

	UFUNCTION()
	void OnRep_CurrentWeapon();
		
};
