// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterAIController.h"
#include "ShooterCharacter.generated.h"

class AGun;
class APickUp;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	int32 DisplayAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	int32 DisplayReserve;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPrecent() const;

	UFUNCTION(BlueprintPure)
	int32 GetCurrentAmmo() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	void Shoot();
	void WeaponSwitchNext();
	void WeaponSwitchLast();
	void Hidder();
	void Reload();
	void AIReload();
	void AIWeaponLoaded();
	bool CheckPlayerLife(AShooterCharacter* Character);

	UFUNCTION(BlueprintCallable)
	void SupplyChar(int32 DropAmmount);

private:
	void MoveForword(float AxisValue);
	void MoveSideWays(float AxisValue);
	void StartSprint();
	void StopSprint();
	void LookUpRate(float AxisValue);
	void LookToSideRate(float AxisValue);

	bool Sprinting;
	bool DiedOnce = false;
	bool status;

	UPROPERTY(EditAnywhere)
	float SprintSpeedModifier = 1.50f;

	UPROPERTY(EditAnywhere)
	USceneComponent* DropSpawnPoint = nullptr;

	UPROPERTY(EditAnywhere)
	float RotationRate = 10;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AGun>> GunClass;

	UPROPERTY()
	AGun* Gun;

	UPROPERTY()
	TArray<AGun*> SpawnedWeapons;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APickUp> AmmoDrop;

	UPROPERTY()
	APickUp* PickUp;

	UPROPERTY()
	TArray<int32> CurrentReserve;

	UPROPERTY()
	TArray<int32> Ammo;

	int32 CurrentMaxAmmo;

	int32 ActiveIndex = 0;
	
};