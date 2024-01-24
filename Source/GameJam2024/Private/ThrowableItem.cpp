// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowableItem.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Hearing.h"


AThrowableItem::AThrowableItem()
{
	AIPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AI Perception Stimuli"));

	
	AIPerceptionStimuliSourceComponent->bAutoRegister=1;
	AIPerceptionStimuliSourceComponent->RegisterForSense(UAISense_Hearing::StaticClass());
}

void AThrowableItem::DetachMeshFromSocket()
{

FDetachmentTransformRules TransformRules(EDetachmentRule::KeepWorld,true);
	ItemMesh->DetachFromComponent(TransformRules);
}

void AThrowableItem::Equip(USceneComponent* InParent, FName InSocketName)
{
	ItemMesh->SetSimulatePhysics(false);
	AttachMeshToSocket(InParent, InSocketName);
	ItemMesh->SetEnableGravity(false);
	ItemMesh->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
	ItemMesh->SetAllPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
}

void AThrowableItem::BeginPlay()
{
	Super::BeginPlay();
	ItemMesh->OnComponentHit.AddDynamic(this,&AThrowableItem::OnSphereHit);
}

void AThrowableItem::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                 FVector NormalImpulse, const FHitResult& Hit)
{
	
	if(DoOnce_Hit&&GetVelocity().Length()>3)
	{
		UE_LOG(LogTemp,Warning,TEXT("Hit"))
		UAISense_Hearing::ReportNoiseEvent(this,Hit.Location,100,this,-1);
		DoOnce_Hit = false;
		//UE_LOG(LogTemp, Warning, TEXT("OnSphereHit called. HitComponent: %s"), *OtherActor->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("OnSphereHit called. velocity %s"), *GetVelocity().ToString());
		//UE_LOG(LogTemp, Warning, TEXT("OnSphereHit called. velocity %f"), GetVelocity().Length());
		PlayHitSound(Hit.Location);

		FTimerHandle ResetHandle;
		GetWorldTimerManager().SetTimer(
			ResetHandle, this, &AThrowableItem::ResetDoOnce, 0.2f, false);

		
	}

	
}

void AThrowableItem::PlayHitSound(const FVector& ImpactPoint)
{
}

void AThrowableItem::ResetDoOnce()
{
	DoOnce_Hit =true;
	UE_LOG(LogTemp,Warning,TEXT("Reset"))
}

void AThrowableItem::AttachMeshToSocket(TObjectPtr<USceneComponent> InParent, FName InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget,true);
	ItemMesh->AttachToComponent(InParent,TransformRules,InSocketName);
	
}

