// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/Scare.h"

#include "Willie.h"
#include "AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UScare::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp,Warning,TEXT("BOOO!"));
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetOwner());
	AWillie* Willie = Cast<AWillie>(EnemyAIController->GetCharacter());
	Willie->Scare();
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
