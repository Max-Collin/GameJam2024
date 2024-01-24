// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "ThrowableItem.generated.h"


class UAIPerceptionStimuliSourceComponent;
/**
 * 
 */
UCLASS()
class GAMEJAM2024_API AThrowableItem : public ABaseItem
{
	GENERATED_BODY()
public:
	AThrowableItem();
	void DetachMeshFromSocket();
	void Equip(USceneComponent* InParent,FName InSocketName);
	
protected:
	
	void BeginPlay() override;
	UFUNCTION()
	void OnSphereHit(UPrimitiveComponent* HitComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& Hit);
	void PlayHitSound(const FVector& ImpactPoint);

	
	UPROPERTY( VisibleAnywhere, Category=Components, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionStimuliSourceComponent>  AIPerceptionStimuliSourceComponent;

	
private:
	bool DoOnce_Hit = true;
	void ResetDoOnce();
	void AttachMeshToSocket(TObjectPtr<USceneComponent>  InParent, FName InSocketName);
	
public:
	FORCEINLINE void Reset_DoOnce_Hit()  { DoOnce_Hit =true;}
};
