// Fill out your copyright notice in the Description page of Project Settings.


#include "P_PlayerCharacter.h"

#include "BaseItem.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ThrowableItem.h"

// Sets default values
AP_PlayerCharacter::AP_PlayerCharacter()
{
	SetRootComponent(GetCapsuleComponent());
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	////CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	//CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera1 = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	
	FollowCamera1->SetupAttachment(RootComponent); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera1->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	LightSource1 = CreateDefaultSubobject<USpotLightComponent>(TEXT("Light Source"));
	LightSource1->SetupAttachment(FollowCamera1);
	FlashLightMesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Flash Light Mesh"));
	FlashLightMesh1->SetupAttachment(FollowCamera1);

	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Capsule to Use"));
	Collision->SetupAttachment(FollowCamera1);

		ThrowStartLocation = CreateDefaultSubobject<UChildActorComponent>(TEXT("ThrowStartLocation"));
    	ThrowStartLocation->SetupAttachment(FollowCamera1);
}

// Called when the game starts or when spawned
void AP_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AP_PlayerCharacter::OnOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AP_PlayerCharacter::OverlapEnd);
	Collision->SetCapsuleRadius(false);
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AP_PlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AP_PlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AP_PlayerCharacter::Mouse1(const FInputActionValue& Value)
{
	
	if(LightSwitch)
	{
		UE_LOG(LogTemp, Warning, TEXT("Off"));
		LightSwitch = false;
		LightSource1->SetVisibility(false);
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("On"));
		LightSwitch = true;
		LightSource1->SetVisibility(true);
	}
	
}



void AP_PlayerCharacter::Aim()
{

	if(!EquippedThrowable)return;
	FPredictProjectilePathParams ProjectilePathParams;
	FPredictProjectilePathResult ProjectilePathResult;


	ProjectilePathParams.StartLocation = ThrowStartLocation->GetComponentLocation()	;
	
	float PlayerPitch ;
	FVector LaunchVelocity;
	if(GetControlRotation().Pitch>=270)
	{
		PlayerPitch = (GetControlRotation().Pitch-270)/110;
	}
	else
	{
		PlayerPitch = (GetControlRotation().Pitch+90)/110;
	}

	
	PlayerPitch = FMath::Clamp(PlayerPitch,0.1,1.2);
	FVector UnitDirection =UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(),ThrowStartLocation->GetComponentLocation());
	//FVector ForwardVector = GetCapsuleComponent()->GetForwardVector();
	LaunchVelocity = UnitDirection* ThrowSpeed ;
	
	// 2x-x^3
	float PitchvelocityX =  PlayerPitch*2-FMath::Cube(PlayerPitch);
	
	LaunchVelocity = FVector(LaunchVelocity.X*PitchvelocityX,LaunchVelocity.Y*PitchvelocityX,FMath::Clamp(FMath::Pow(LaunchVelocity.Z,PlayerPitch),0,800));
	
	
	ProjectilePathParams.LaunchVelocity = LaunchVelocity;
	ProjectilePathParams.SimFrequency = 30;
	ProjectilePathParams.DrawDebugType = EDrawDebugTrace::ForOneFrame;
	ProjectilePathParams.bTraceWithCollision =true;
	
	UGameplayStatics::PredictProjectilePath(this,ProjectilePathParams,ProjectilePathResult);
	//UE_LOG(LogTemp,Warning,TEXT("Aim"));

	
	FString String = FString::Printf(TEXT("%f"),GetControlRotation().Pitch);
	FString String2 = FString::Printf(TEXT("%f"),PlayerPitch);
	FString String3 = FString::Printf(TEXT("%f"),FMath::Pow(LaunchVelocity.Z,PlayerPitch));
	FString String4 = FString::Printf(TEXT("%s"),*LaunchVelocity.ToString());
	
	
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1,0.1f,FColor::Red,String);
		GEngine->AddOnScreenDebugMessage(2,0.1f,FColor::Blue,String2);
		GEngine->AddOnScreenDebugMessage(3,0.1f,FColor::Green,String3);
		GEngine->AddOnScreenDebugMessage(4,0.1f,FColor::Green,String4);
	}

	//FVector(ForwardVector.X,ForwardVector.Y,ForwardVector.Z=50)
}

void AP_PlayerCharacter::Throw()
{

	
		if(!EquippedThrowable)return;
		EquippedThrowable->GetMesh()->SetSimulatePhysics(true);
		EquippedThrowable->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		EquippedThrowable->Reset_DoOnce_Hit();
		EquippedThrowable->DetachMeshFromSocket();
	
		float PlayerPitch ;
		FVector LaunchVelocity;
		if(GetControlRotation().Pitch>=270)
		{
			PlayerPitch = (GetControlRotation().Pitch-270)/110;
			//FVector ForwardVector = GetCapsuleComponent()->GetForwardVector();
			//LaunchVelocity = ForwardVector* ThrowSpeed *PlayerPitch +10 ;
		}
		else
		{
			PlayerPitch = (GetControlRotation().Pitch+90)/110;
		
	
		}
		PlayerPitch = FMath::Clamp(PlayerPitch,0.01,1.2);
		FVector UnitDirection =UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(),ThrowStartLocation->GetComponentLocation());
		FVector ForwardVector = GetCapsuleComponent()->GetForwardVector();
		LaunchVelocity = UnitDirection* ThrowSpeed ;
		float PitchvelocityX =  PlayerPitch*2-FMath::Cube(PlayerPitch);
		LaunchVelocity = FVector(LaunchVelocity.X*PitchvelocityX,LaunchVelocity.Y*PitchvelocityX,FMath::Pow(LaunchVelocity.Z,PlayerPitch));
	
		EquippedThrowable->GetMesh()->SetPhysicsLinearVelocity(LaunchVelocity);
		EquippedThrowable->GetMesh()->SetEnableGravity(true);
		EquippedThrowable = nullptr;
	
	
	
	
}

void AP_PlayerCharacter::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                   int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap"));
	ItemToPickup = Cast<ABaseItem>(OtherActor);
	if(ItemToPickup)
	{
		Interacting = true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Interact?: %s"), ( Interacting ? TEXT("true"): TEXT("false") ));
}

void AP_PlayerCharacter::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap Leave"));
	ItemToPickup = Cast<ABaseItem>(OtherActor);
	if(ItemToPickup)
	{
		Interacting = false;
	}
	UE_LOG(LogTemp, Warning, TEXT("Interact?: %s"), ( Interacting ? TEXT("true"): TEXT("false") ));
}

void AP_PlayerCharacter::Interact(const FInputActionValue& Value)
{
	Interacting = Value.Get<bool>();
	
	
	if (Controller != nullptr)
	{
		if(Interacting)
		{
			UE_LOG(LogTemp, Warning, TEXT("Destroy"));
			if(ItemToPickup)
			{
				ItemToPickup->Interact(this);
			}
		}
	}
}

// Called every frame
void AP_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AP_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AP_PlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AP_PlayerCharacter::Look);

		//Mouse 1ing
		EnhancedInputComponent->BindAction(Mouse1Action, ETriggerEvent::Triggered, this, &AP_PlayerCharacter::Mouse1);

		//Eing
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AP_PlayerCharacter::Interact);

		EnhancedInputComponent->BindAction(AimAction,ETriggerEvent::Triggered,this,&AP_PlayerCharacter::Aim);
		EnhancedInputComponent->BindAction(AimAction,ETriggerEvent::Completed,this,&AP_PlayerCharacter::Throw);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

