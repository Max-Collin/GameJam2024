// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Logging/LogMacros.h"
#include "P_PlayerCharacter.generated.h"

class AThrowableItem;
class USpotLightComponent;
class UPointLightComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UChildActorComponent;
class ABaseItem;

//CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class GAMEJAM2024_API AP_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera1;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* Collision;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Mouse1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere,Category = Input)
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditAnywhere)
	USpotLightComponent* LightSource1;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* FlashLightMesh1;
	
	UPROPERTY(EditAnywhere)
	ABaseItem* ItemToPickup1;

	
	
public:
	// Sets default values for this pawn's properties
	AP_PlayerCharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	//FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera1; }

	TObjectPtr<AThrowableItem> EquippedThrowable;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowInteractMessage();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowHUDCollectedEnoughLootMessage();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowHUDWinMessage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CollectedLootValue = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetLootValue = 10;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	USoundBase* FlashLightOn;

	UPROPERTY(EditAnywhere)
	USoundBase* FlashLightOff;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for Mouse1 input */
	void Mouse1(const FInputActionValue& Value);
	
	void Interact(const FInputActionValue& Value);

	

	void Aim();
	void Throw();

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool LightSwitch = true;
	bool Interacting = false;
	bool inRangeToPickup = false;

	
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UChildActorComponent> ThrowStartLocation;
	UPROPERTY(EditAnywhere)
	float ThrowSpeed=800;
private:
	

};
