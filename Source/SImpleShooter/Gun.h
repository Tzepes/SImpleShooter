// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGun();

	void PullTrigger();
	void ChangeMagazine();

	UFUNCTION(BlueprintCallable)
	void Supply(int32 DropAmmount);

	UFUNCTION(BlueprintCallable)
	bool IsAmmoReserveMAX();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo variables")
	int32 MaxReserve = 180;

	UPROPERTY()
	int32 CurrentReserve = MaxReserve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo variables")
	int32 MaxAmmo = 30;

	UPROPERTY()
	int32 Ammo = MaxAmmo;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* BulletHit;

	UPROPERTY(EditAnywhere)
	USoundBase* BulletSound;

	UPROPERTY(EditAnywhere)
	USoundBase* EmptyGunClick;

	UPROPERTY(EditAnywhere)
	USoundBase* ReloadSound;

	UPROPERTY(EditAnywhere)
	float MaxRange = 100000;

	UPROPERTY(EditAnywhere)
	float Damage = 10;

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	AController* GetOwnerController() const;	
};