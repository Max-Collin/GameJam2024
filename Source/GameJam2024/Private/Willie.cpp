// Fill out your copyright notice in the Description page of Project Settings.


#include "Willie.h"
#include "Engine/TargetPoint.h"
#include "AI/EnemyAIController.h"

// Sets default values
AWillie::AWillie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AWillie::Scare()
{
	if(ScareCount<=1)
	{
	UE_LOG(LogTemp,Warning,TEXT("Second Booo"));
	}
	else
	{
		ScareCount++;
	}
}

// Called when the game starts or when spawned
void AWillie::BeginPlay()
{
	Super::BeginPlay();
	BlackboardComponent = Cast<AEnemyAIController>(GetController())->GetBlackboardComponent();
	
	
}

// Called every frame
void AWillie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWillie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void AWillie::NewPatrolTarget()
{
	UE_LOG(LogTemp,Warning,TEXT("new Patrol Target"))
	TArray<ATargetPoint*> PatrolPointsTemp= PatrolPoints;
	//PatrolPointsTemp.RemoveAt(PatrolPointsTemp.Find(CurrentPatrolTarget));
	ATargetPoint* NewPatrolPoint = PatrolPointsTemp[FMath::RandRange(0,PatrolPoints.Num()-1)];



	if(BlackboardComponent)
	{
		BlackboardComponent->SetValueAsVector("PatrolPoint",NewPatrolPoint->GetActorLocation());
		CurrentPatrolTarget = NewPatrolPoint;
	}

	
}

