// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Character.h"
#include "Willie.generated.h"

UCLASS()
class GAMEJAM2024_API AWillie : public ACharacter
{
	GENERATED_BODY()
	TObjectPtr<UBlackboardComponent> BlackboardComponent;

public:
	// Sets default values for this character's properties
	AWillie();
	UPROPERTY(VisibleAnywhere)
	TArray<ATargetPoint*> PatrolPoints;
	void NewPatrolTarget();
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ATargetPoint> CurrentPatrolTarget;
    void Scare();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private: 
	int32 ScareCount =0;

};
