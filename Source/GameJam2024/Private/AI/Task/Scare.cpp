// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/Scare.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UScare::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp,Warning,TEXT("BOOO!"));

	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
