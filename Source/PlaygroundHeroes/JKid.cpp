// Fill out your copyright notice in the Description page of Project Settings.

#include "JKid.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "JEnemy.h"
#include "EngineUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Engine.h"

// Sets default values
AJKid::AJKid()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	PrimaryActorTick.bCanEverTick = true;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	LockCamRate = .05f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	bCrawling = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	InputDirection = FVector(0.f, 0.f, 0.f);
	MovementModifier = 1.0f;
	bCameraInverted = false;
}

// Called when the game starts or when spawned
void AJKid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJKid::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AJKid::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	//PlayerInputComponent->BindAction("LockCam", IE_Pressed, this, &AJHero::LockCamera);
	//PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AJHero::Attack);
	//PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AJHero::Dodge);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AJKid::InteractPressed);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &AJKid::InteractReleased);

	PlayerInputComponent->BindAxis("MoveForward", this, &AJKid::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AJKid::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AJKid::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AJKid::LookUpAtRate);
}

void AJKid::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AJKid::LookUpAtRate(float Rate)
{
	if (bCameraInverted)
		Rate *= -1.f;
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AJKid::MoveForward(float Value)
{
	// I know this is wrong but if I flip it it breaks so leave it
	InputDirection.X = Value;
	Value *= MovementModifier;
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		//UE_LOG(LogClass, Warning, TEXT("MoveForward"));
		//GetCharacterMovement()->AddForce(Direction*Value);
	}
}

void AJKid::MoveRight(float Value)
{
	// I know this is wrong but if I flip it it breaks so leave it
	InputDirection.Y = Value;
	Value *= MovementModifier;
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
		//GetCharacterMovement()->AddForce(Direction*Value);
	}
}

void AJKid::InteractPressed()
{
	interacting = true;
}

void AJKid::InteractReleased()
{
	interacting = false;
}