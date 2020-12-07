// Fill out your copyright notice in the Description page of Project Settings.



#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

void AGun::PullTrigger()
{
	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);
	if (Ammo > 0) {
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));
		if (bSuccess) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletHit, Hit.Location, ShotDirection.Rotation());
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), BulletSound, Hit.Location, ShotDirection.Rotation());

			AActor* HitActor = Hit.GetActor();
			if (Hit.GetActor() != nullptr) {
				FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
				AController* OwnerController = GetOwnerController();
				HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
			}
			Ammo--;
			UE_LOG(LogTemp, Warning, TEXT("AMMO %i / %i"), Ammo, AmmoReserve);
		}
	}
	else {
		UGameplayStatics::SpawnSoundAttached(EmptyGunClick, Mesh, TEXT("MuzzleFlashSocket"));
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();

	Ammo = MaxAmmo;
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection) {
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) 
		return false;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * MaxRange;

	Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) 
		return nullptr;

	return OwnerPawn->GetController();
}

void AGun::ChangeMagazine() {
	int32 NeededAmmo = MaxAmmo - Ammo;
	UE_LOG(LogTemp, Warning, TEXT("RELOADING!"));	
	UGameplayStatics::SpawnSoundAttached(ReloadSound, Mesh, TEXT("MuzzleFlashSocket"));
	if (NeededAmmo <= AmmoReserve) {
		AmmoReserve = AmmoReserve - NeededAmmo;
	}
	else {
		NeededAmmo = AmmoReserve;
		AmmoReserve = 0;
	}
	Ammo = Ammo + NeededAmmo;
	UE_LOG(LogTemp, Warning, TEXT("AMMO %i / %i"), Ammo, AmmoReserve);
}

void AGun::Supply() {
	if (MaxReserve - AmmoReserve > 20) {
		AmmoReserve += 20;
	}
	else {
		AmmoReserve += MaxReserve - AmmoReserve;
	}
		
	UE_LOG(LogTemp, Warning, TEXT("AMMUNITON WAS ADDED"));
	UE_LOG(LogTemp, Warning, TEXT("AMMO %i / %i"), Ammo, AmmoReserve);
}
