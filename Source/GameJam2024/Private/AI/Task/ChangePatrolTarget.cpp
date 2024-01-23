// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/ChangePatrolTarget.h"

#include "Willie.h"
#include "AI/EnemyAIController.h"

EBTNodeResult::Type UChangePatrolTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetOwner());
	AWillie* Willie = Cast<AWillie>(EnemyAIController->GetCharacter());
	Willie->NewPatrolTarget();

	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
