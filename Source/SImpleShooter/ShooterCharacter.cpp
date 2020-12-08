// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "PickUp.h"
#include "Components/CapsuleComponent.h"
#include "SImpleShooterGameModeBase.h"
#include "EngineUtils.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DropSpawnPoint = CreateDefaultSubobject<USceneComponent>("DropSpawnPoint");
	DropSpawnPoint->SetupAttachment(GetCapsuleComponent());
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	for (TSubclassOf<AGun> GunIndex : GunClass) {
		
		Gun = GetWorld()->SpawnActor<AGun>(GunIndex);
		if (Gun == nullptr) {
			UE_LOG(LogTemp, Error, TEXT("Gun has nullptr"));
			return;
		}
		SpawnedWeapons.Add(Gun);
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Gun->SetOwner(this);
		Gun->SetActorHiddenInGame(true);
		CurrentReserve.Add(Gun->MaxReserve);
		Ammo.Add(Gun->MaxAmmo);
	}
	SpawnedWeapons[0]->SetActorHiddenInGame(false);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);	
	DisplayAmmo = Ammo[ActiveIndex];
	//DisplayReserve = CurrentReserve[ActiveIndex];
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForword"), this, &AShooterCharacter::MoveForword);
	PlayerInputComponent->BindAxis(TEXT("MoveSideWays"), this, &AShooterCharacter::MoveSideWays);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookToSide"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookToSideRate"), this, &AShooterCharacter::LookToSideRate);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("WeaponSwitchNext"), EInputEvent::IE_Pressed, this, &AShooterCharacter::WeaponSwitchNext);
	PlayerInputComponent->BindAction(TEXT("WeaponSwitchLast"), EInputEvent::IE_Pressed, this, &AShooterCharacter::WeaponSwitchLast);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Reload);
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;

	if (IsDead()) {
		FVector Loc = DropSpawnPoint->GetComponentLocation();
		FRotator Rot = DropSpawnPoint->GetComponentRotation();

		ASImpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASImpleShooterGameModeBase>();
		if (GameMode != nullptr) {
			GameMode->PawnKilled(this);
		}
		if (!DiedOnce) {
			DetachFromControllerPendingDestroy();
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			PickUp = GetWorld()->SpawnActor<APickUp>(AmmoDrop, Loc, Rot);
		}
		DiedOnce = true;
	}

	return DamageToApply;
}

void AShooterCharacter::MoveForword(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveSideWays(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookToSideRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Shoot()
{
	Gun = SpawnedWeapons[ActiveIndex];
	Gun->PullTrigger();
	if (Ammo[ActiveIndex] > 0) {
		Ammo[ActiveIndex]--;
		DisplayAmmo = Ammo[ActiveIndex];
	}
}

void AShooterCharacter::WeaponSwitchNext()
{
	if (ActiveIndex >= SpawnedWeapons.Num() - 1) {
		ActiveIndex = 0;
	}
	else {
		ActiveIndex++;
	}
	Hidder();
	DisplayAmmo = Ammo[ActiveIndex];
}

void AShooterCharacter::WeaponSwitchLast() {
	if (ActiveIndex <= 0) {
		ActiveIndex = SpawnedWeapons.Num() - 1;
	} else {
		ActiveIndex--;
	}
	Hidder();
	DisplayAmmo = Ammo[ActiveIndex];
}

float AShooterCharacter::GetHealthPrecent() const {
	return Health / MaxHealth;
}

int32 AShooterCharacter::GetCurrentAmmo() const {
	return Gun->Ammo;
}

void AShooterCharacter::Hidder() {
	for (int32 Index = 0; Index < SpawnedWeapons.Num(); Index++) {
		if (Index != ActiveIndex) {
			SpawnedWeapons[Index]->SetActorHiddenInGame(true);
		}
	}
	SpawnedWeapons[ActiveIndex]->SetActorHiddenInGame(false);
}

void AShooterCharacter::Reload()
{
	if (Ammo[ActiveIndex] != CurrentReserve[ActiveIndex]) {
		Gun->ChangeMagazine();
		Ammo[ActiveIndex] = SpawnedWeapons[ActiveIndex]->MaxAmmo;
		DisplayAmmo = Ammo[ActiveIndex];
	}
}

void AShooterCharacter::SupplyChar(int32 DropAmmount) {
	Gun->Supply(DropAmmount);
}
