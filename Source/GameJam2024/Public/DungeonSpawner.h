// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonSpawner.generated.h"

class UItemSpawner;

UCLASS()
class GAMEJAM2024_API ADungeonSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonSpawner();
	virtual void OnConstruction(const FTransform& Transform) override;

	void NewMap();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UItemSpawner> SpawnerComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere)
	bool GenerateRoof = false;
	
	UPROPERTY(EditAnywhere , Category = "Map Settings")
	int32 Seed;
	
	UPROPERTY(EditAnywhere , Category = "Map Settings")
	int32 RoomSize_Min=3;

	UPROPERTY(EditAnywhere , Category = "Map Settings")
	int32 RoomSize_Max=9;

	UPROPERTY(EditAnywhere , Category = "Map Settings")
	int32 RoomCount=1;
	
	UPROPERTY(EditAnywhere , Category = "Map Settings")
	int32 FloorCull_Min=0;

	UPROPERTY(EditAnywhere , Category = "Map Settings")
	int32 FloorCull_Max=1;

	UPROPERTY(EditAnywhere , Category = "Map Settings")
	bool bFloorCulling = false;

	UPROPERTY(EditAnywhere , Category = "Map Settings")
	int32 BranchingThreshold;

	UPROPERTY(EditAnywhere , Category = "Map Settings")
	float BranchingChance;

	UPROPERTY(EditAnywhere , Category = "Map Settings")
	bool Merging;
	
	UPROPERTY(EditAnywhere , Category = "Map Settings")
	bool Branching;


	UPROPERTY(EditAnywhere , Category = "Editor Settings")
	int32 MaxLoops;

	UPROPERTY(EditAnywhere , Category = "Editor Settings")
	float Scale;

	UPROPERTY(EditAnywhere , Category = "Editor Settings")
	bool NewSeed = true;
	
	UPROPERTY(EditAnywhere)
	FRandomStream Stream;

	UPROPERTY(EditAnywhere)
	TArray<FIntVector> RoomOnlyTiles;
	UPROPERTY(EditAnywhere)
	TArray<FIntVector> FloorTiles;
	UPROPERTY(EditAnywhere)
	TArray<FIntVector> CorridorTiles;

	//Meshes
	UPROPERTY(EditAnywhere,BlueprintReadWrite , Category = "Meshes ")
	UInstancedStaticMeshComponent* FloorMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite , Category = "Meshes ")
	UInstancedStaticMeshComponent* WallMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite , Category = "Meshes ")
	UInstancedStaticMeshComponent* RoofMesh;

	


private:
	
	void GenerateMap();
	void Room_GenerateFloor(FIntVector RootLocation );
	void SpawnWalls(bool IsFloorTile, UE::Math::TIntVector3<int> FloorTile, FVector SpawnLocation);
	void SpawnTiles();
	bool Room_FindNextLocation(FIntVector &NewLocation);
	FIntVector TestRelativeTileLocation(FIntVector ReferenceLocation,int32 X,int32 Y, bool &thisISFloorTile);

	TArray<FIntVector> Corridors_MapCorridors(FIntVector RoomA , FIntVector RoomB);
	TArray<FIntVector> Corridors_MakeY(FIntVector From , FIntVector To);
	TArray<FIntVector> Corridors_MakeX(FIntVector From , FIntVector To);
	
	FIntVector MaxExtents;
	UPROPERTY(VisibleAnywhere)
	TMap<FIntVector,FIntVector> Rooms;
	
	FIntVector PreviousLocation = FIntVector(0,0,0);
	
public:	


	
	FORCEINLINE TArray<FIntVector> GetFloorTiles() const {return RoomOnlyTiles;}
	FORCEINLINE TArray<FIntVector> GetCorridorTiles() const {return CorridorTiles;}
	FORCEINLINE FRandomStream GetStream() const {return Stream;}
	FORCEINLINE TMap<FIntVector,FIntVector> GetRooms() const { return Rooms;}
	FORCEINLINE float GetScale() const {return Scale;}

};
