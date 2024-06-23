// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "BaseWeapon.generated.h"

UCLASS()
class UE5TPSONLINE_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USceneComponent* RootComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire(); // Server-side function declaration

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ReleaseFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ReleaseServerFire(); // Server-side function declaration

	void Reload();

	void FinishReload();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReload();

	void SetCanFire();

	UFUNCTION()
	void OnRep_Ammo();

	UFUNCTION()
	void OnRep_Reloading();

	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetAmmo(int32& lMaxAmmo);

	void SetDefaultAmmo();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float Damage = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireDelay = 0.2f;

	FTimerHandle TimerHandleDelay;
	
	bool bCanFire = true;

	bool bButtonPressed = false;

	UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = "OnRep_Reloading")
	bool bReloading;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	bool bAutofire;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int32 MaxAmmo;

	UPROPERTY(Replicated, ReplicatedUsing = "OnRep_Ammo")
	int32 Ammo = MaxAmmo;

	FTimerHandle TimerHandleReload;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float ReloadTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float Range = 1000.0f;

};
