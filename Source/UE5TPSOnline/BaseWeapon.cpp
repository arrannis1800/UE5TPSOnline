// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Camera/CameraComponent.h"
#include "UE5TPSOnlineCharacter.h"
#include "Engine/DamageEvents.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
    SetRootComponent(RootComponent);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
    Mesh->SetupAttachment(RootComponent);
    Mesh->SetMobility(EComponentMobility::Movable);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Mesh->SetGenerateOverlapEvents(false);

}

void ABaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseWeapon, Ammo);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeapon::Fire()
{
    if (!bCanFire)
        return;

    if (HasAuthority()) // Ensure we have server authority
    {
        if (Ammo <= 0)
        {

            UE_LOG(LogTemplateCharacter, Error, TEXT("Reload"));
            GetWorld()->GetTimerManager().SetTimer(TimerHandleReload, this, &ABaseWeapon::Reload, ReloadTime);
            return;
        }
        --Ammo;
        ServerFire(); // Call server-side function via RPC
    }
}

void ABaseWeapon::ServerFire_Implementation()
{

    UCameraComponent* CameraC = Cast<AUE5TPSOnlineCharacter>(GetOwner())->GetFollowCamera();

    if (CameraC)
    {
        bCanFire = false;
        bButtonPressed = true;

        FHitResult HResult;
        FVector Start = CameraC->GetComponentLocation();
        FVector End = Start + CameraC->GetForwardVector() * Range;
        FCollisionQueryParams QParams;
        QParams.AddIgnoredActor(GetOwner());
        QParams.AddIgnoredActor(this);
        GetWorld()->LineTraceSingleByChannel(HResult, Start, End, ECollisionChannel::ECC_Visibility, QParams, FCollisionResponseParams());

        DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0);

        if (HResult.bBlockingHit)
        {
            AUE5TPSOnlineCharacter* Actor = Cast<AUE5TPSOnlineCharacter>(HResult.GetActor());
            if (Actor)
            {
                AController* Controller = Cast<AUE5TPSOnlineCharacter>(GetOwner())->GetInstigatorController();
                if (Controller)
                {
                    FPointDamageEvent PointDamageEvent(Damage, HResult, HResult.TraceEnd, UDamageType::StaticClass());
                    Actor->TakeDamage(Damage, PointDamageEvent, Controller, this);
                }
                UE_LOG(LogTemp, Error, TEXT("%s"), *HResult.GetActor()->GetName());
            }
        }

        if (bAutofire)
        {
            GetWorld()->GetTimerManager().SetTimer(TimerHandleDelay, this, &ABaseWeapon::SetCanFire, FireDelay);
        }
    }
}

bool ABaseWeapon::ServerFire_Validate()
{
    return true;
}

void ABaseWeapon::ReleaseFire()
{
    if (HasAuthority()) // Ensure we have server authority
    {
        ReleaseServerFire();
        SetCanFire();
    }
}

void ABaseWeapon::ReleaseServerFire_Implementation()
{
    bButtonPressed = false;
}

bool ABaseWeapon::ReleaseServerFire_Validate()
{
    return true;
}

void ABaseWeapon::Reload()
{
    Ammo = MaxAmmo;
    bCanFire = true;
}

void ABaseWeapon::SetCanFire()
{
    bCanFire = true;
}

int32 ABaseWeapon::GetAmmo(int32& lMaxAmmo)
{
    lMaxAmmo = MaxAmmo;
    return Ammo;
}

void ABaseWeapon::OnRep_Ammo()
{

}

