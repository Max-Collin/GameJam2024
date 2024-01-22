// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemSpawner.generated.h"


class ADungeonSpawner;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEJAM2024_API UItemSpawner : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemSpawner();

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

private:
	void Spawn();
	void SpawnItems();
	void SpawnEndDoor();

	float Scale;
	FRandomStream Stream;

	UPROPERTY(VisibleAnywhere)
	TArray<FIntVector> SpawnLocationTiles;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ADungeonSpawner> DungeonSpawner;
	
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
