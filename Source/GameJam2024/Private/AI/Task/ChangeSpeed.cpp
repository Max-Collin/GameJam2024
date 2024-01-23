// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/ChangeSpeed.h"

#include "AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UChangeSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	

	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetOwner());
	
	EnemyAIController->ChangeSpeed(true);

	return Super::ExecuteTask(OwnerComp, NodeMemory);
	
}
