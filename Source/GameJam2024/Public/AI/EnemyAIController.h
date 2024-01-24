// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */


class ATargetPoint;
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
		TObjectPtr<UAISenseConfig_Hearing> Hear;
	
public:

void GetRandomLocation();

	AEnemyAIController();
	void ChangeSpeed(bool Walking);
	

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
