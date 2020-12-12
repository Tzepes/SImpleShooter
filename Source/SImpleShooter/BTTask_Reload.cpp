// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Reload.h"
#include "AIController.h"
#include <SImpleShooter\ShooterCharacter.h>

UBTTask_Reload::UBTTask_Reload() {
	NodeName = "Reload";
}

EBTNodeResult::Type UBTTask_Reload::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr) {
		return EBTNodeResult::Failed;
	}

	AShooterCharacter* Character = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr) {
		return EBTNodeResult::Failed;
	}

	Character->Reload();
	Character->AIWeaponLoaded();
	return EBTNodeResult::Succeeded;
}
