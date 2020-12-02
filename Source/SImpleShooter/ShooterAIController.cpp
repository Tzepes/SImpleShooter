// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "SImpleShooterGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"


void AShooterAIController::BeginPlay() {
	Super::BeginPlay();
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//ControlledCharacter;
	
	if (AIBehavior != nullptr) {
		RunBehaviorTree(AIBehavior);		
		
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsPlayerAlive"), true);
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsPlayerDead"), false);
}

void AShooterAIController::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}

bool AShooterAIController::IsDead() const {
	AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
	if (ControlledCharacter != nullptr) {
		return ControlledCharacter->IsDead();
	}
	return true;
}

void AShooterAIController::PlayerDied() {
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsPlayerAlive"), false);
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsPlayerDead"), true);
}



