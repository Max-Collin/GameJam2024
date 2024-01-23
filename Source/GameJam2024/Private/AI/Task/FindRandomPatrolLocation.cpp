// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/FindRandomPatrolLocation.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Willie.h"
#include "AI/EnemyAIController.h"


EBTNodeResult::Type UFindRandomPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	

	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetOwner());
	AWillie* Willie = Cast<AWillie>(EnemyAIController->GetCharacter());
	if(EnemyAIController&&Willie)
	{
		UNavigationSystemV1* NavigationSystemV1 = UNavigationSystemV1::GetCurrent(GetWorld());
		FNavLocation NavLocation;

		if(NavigationSystemV1->GetRandomReachablePointInRadius(Willie->GetActorLocation(),10000.f,NavLocation))
		{
			EnemyAIController->GetBlackboardComponent()->SetValueAsVector("RandomLocation",NavLocation.Location);

			FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
		
		
	}

	FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
	
	
}
