// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawner.h"

#include "DungeonSpawner.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UItemSpawner::UItemSpawner()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	

	DungeonSpawner = Cast<ADungeonSpawner>(GetOwner());
	Stream = DungeonSpawner->GetStream();
	Scale = DungeonSpawner->GetScale();
	if(DungeonSpawner)
	{
		SpawnLocationTiles = DungeonSpawner->GetFloorTiles();
	}	
	SpawnEndDoor();
	Spawn();
	// ...
	
}


void UItemSpawner::Spawn()
{
	

		if(!RoomOnly)
		{
			SpawnLocationTiles.Append(DungeonSpawner->GetCorridorTiles());
		}
		SpawnItems();
	
}
void UItemSpawner::SpawnItems()
{
	
	int32 ItemsSpawned=0;
	int32 FloorIndex;
	while (ItemsSpawned<Quantity)
	{
		FloorIndex = Stream.RandRange(0,SpawnLocationTiles.Num()-1);
		FVector FloorIndexVector = UKismetMathLibrary::Conv_IntVectorToVector(SpawnLocationTiles[FloorIndex]);
		FVector TempVector = UKismetMathLibrary::Add_VectorVector(FloorIndexVector *Scale,DungeonSpawner->GetActorLocation());
		FVector SpawnLocationVector = UKismetMathLibrary::Add_VectorVector(TempVector,offset);
		TArray< TSubclassOf<AActor>> Keys;
		SpawnList.GetKeys(Keys);
		
		for (auto Key : Keys)
		{
			
			bool bSpawn = UKismetMathLibrary::RandomBoolWithWeightFromStream(Stream,*SpawnList.Find(Key));
			if(bSpawn&&GetWorld())
			{
				
				AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(Key,SpawnLocationVector,FRotator(0.f,0.f,0.f));

			
				SpawnLocationTiles.RemoveAt(FloorIndex);
				ItemsSpawned++;
				break;
			}
		}
	}


}

void UItemSpawner::SpawnEndDoor()
{


	FVector FloorIndexVector = UKismetMathLibrary::Conv_IntVectorToVector(SpawnLocationTiles[SpawnLocationTiles.Num()-1]);
	FVector TempVector = UKismetMathLibrary::Add_VectorVector(FloorIndexVector *Scale,DungeonSpawner->GetActorLocation());
	FVector SpawnLocationVector = UKismetMathLibrary::Add_VectorVector(TempVector,offset);

	GetWorld()->SpawnActor<AActor>(EndDoor,SpawnLocationVector,FRotator(0.f,0.f,0.f));
	SpawnLocationTiles.RemoveAt(SpawnLocationTiles.Num()-1);
}


// Called every frame
void UItemSpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



