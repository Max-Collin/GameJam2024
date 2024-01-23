// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/ChangeSpeed2.h"

#include "AI/EnemyAIController.h"

EBTNodeResult::Type UChangeSpeed2::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetOwner());
	
	EnemyAIController->ChangeSpeed(false);

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
