// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AI/NavigationSystemBase.h"
#include "NavigationSystem.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Willie.h"
#include "Perception/AIPerceptionTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionSystem.h"

void AEnemyAIController::GetRandomLocation()
{
	if(BlackboardComponent)
	{
	
	
	
	}
}

AEnemyAIController::AEnemyAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component")));
	
	

	Sight=CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	
	Sight->SightRadius=2000.f;
	Sight->LoseSightRadius=1500.f;
	Sight->PeripheralVisionAngleDegrees = 60.f;
	Sight->DetectionByAffiliation.bDetectEnemies=true;
	Sight->DetectionByAffiliation.bDetectNeutrals=true;
	Sight->DetectionByAffiliation.bDetectFriendlies=true;

	Hear = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	Hear->HearingRange=1000.f;
	Hear->DetectionByAffiliation.bDetectEnemies=true;
	Hear->DetectionByAffiliation.bDetectNeutrals=true;
	Hear->DetectionByAffiliation.bDetectFriendlies=true;

	AAIController::GetPerceptionComponent()->ConfigureSense(*Sight);
	AAIController::GetPerceptionComponent()->ConfigureSense(*Hear);
	
	AAIController::GetPerceptionComponent()->SetDominantSense(*Sight->GetSenseImplementation());
	AAIController::GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this,&AEnemyAIController::OnTargetPerceptionUpdate);
}

void AEnemyAIController::ChangeSpeed(bool Walking)
{
	AWillie* Willie = Cast<AWillie>(GetCharacter());
	if(Walking)
	{
	
		Willie->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		
	}
	else
	{
		
		Willie->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
	
}



void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();


	if(BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
	if(BlackboardComponent && BlackboardData)
	{
		InitializeBlackboard(*BlackboardComponent,*BlackboardData);
	}
}

void AEnemyAIController::OnTargetPerceptionUpdate(AActor* SeenActor, FAIStimulus Stimulus)
{
	if(SeenActor)
	{
		if(Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>()&&Stimulus.WasSuccessfullySensed())
		{
			
			if(BlackboardComponent)
			{
				UE_LOG(LogTemp,Warning,TEXT("chasing %s"),*SeenActor->GetName())
				BlackboardComponent->SetValueAsObject("Player",SeenActor);
			
			}
        		
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Lost %s"),*SeenActor->GetName())
			BlackboardComponent->SetValueAsObject("Player",nullptr);
			BlackboardComponent->SetValueAsVector("LastKnowLocation" ,Stimulus.StimulusLocation);
			
			UE_LOG(LogTemp,Warning,TEXT("Checking Last location %s"),*Stimulus.StimulusLocation.ToString())
		}
		if(Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
		{
			if(BlackboardComponent)
			{
				BlackboardComponent->SetValueAsVector("InvestigationTarget",Stimulus.StimulusLocation);
				
			}
		}
	}
}
