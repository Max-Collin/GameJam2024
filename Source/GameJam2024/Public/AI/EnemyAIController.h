// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
 
 
 class UAISenseConfig_Sight;
 class UAISenseConfig_Hearing;
 struct FAIStimulus;
 
UCLASS()
class GAMEJAM2024_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
    	TObjectPtr<UBehaviorTree> BehaviorTree;
    	
    	TObjectPtr<UBlackboardComponent> BlackboardComponent;
    	UPROPERTY(EditDefaultsOnly)
    	TObjectPtr<UBlackboardData> BlackboardData;
    
    	TObjectPtr<class UAIPerceptionComponent> UaiPerceptionComponent;
    	
    	TObjectPtr<class UAISenseConfig_Sight> Sight;
	
public:
	
	
void GetRandomLocation();
	virtual void Tick(float DeltaSeconds) override;
	AEnemyAIController();
	void ChangeSpeed(bool Walking);
	void LostSight();
	void StartChase();
	bool Chasing=false;
	

protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void OnTargetPerceptionUpdate(AActor* SeenActor,FAIStimulus Stimulus);
	
	bool IsWalking = true;

	UPROPERTY(EditAnywhere)
	float WalkSpeed;
	UPROPERTY(EditAnywhere)
	float RunSpeed;
};
