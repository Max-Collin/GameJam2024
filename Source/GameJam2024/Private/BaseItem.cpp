// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"

#include "P_PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	RootComponent = ItemMesh;
	
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionSphere->SetupAttachment(RootComponent);
}

void ABaseItem::Interact(AActor* InteractingActor)
{
	SelfDestruct();
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseItem::SelfDestruct()
{
	this->Destroy();
}

