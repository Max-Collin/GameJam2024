// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemSpawner.generated.h"


class AWillie;
class ATargetPoint;class ADungeonSpawner;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEJAM2024_API UItemSpawner : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemSpawner();
	void OnDungeonSpawn();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	FVector offset;
	
	UPROPERTY(EditAnywhere)
	int32 Quantity=0;
	
	UPROPERTY(EditAnywhere)
	bool RoomOnly = true;
	
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<AActor>,float> SpawnList;


	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> EndDoor;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATargetPoint> Targetpoint;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWillie> Willie;

private:
	void Spawn();
	void SpawnItems();
	void SpawnWillie();
	void SpawnEndDoor();
	void SpawnPatrolPoints();

	float Scale;
	FRandomStream Stream;

	UPROPERTY(VisibleAnywhere)
	TArray<FIntVector> SpawnLocationTiles;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ADungeonSpawner> DungeonSpawner;
	UPROPERTY(VisibleAnywhere)
	TArray<ATargetPoint*> PatrolLocations;
	
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
