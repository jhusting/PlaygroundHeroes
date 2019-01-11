// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "PlaygroundHeroesCharacter.h"
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

//////////////////////////////////////////////////////////////////////////
// APlaygroundHeroesCharacter

APlaygroundHeroesCharacter::APlaygroundHeroesCharacter()
{
	// Set size for collision capsule
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

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
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

	//Create the right sphere
	/*RightHand = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandSphere"));
	RightHand->AttachTo(GetMesh(), TEXT("RightHandSocket"), EAttachLocation::SnapToTarget, true);
	RightHand->SetSphereRadius(10.f);
	RightHand->bGenerateOverlapEvents = true;

	//Create the left sphere
	LeftHand = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandSphere"));
	LeftHand->AttachTo(GetMesh(), TEXT("LeftHandSocket"), EAttachLocation::SnapToTarget, true);
	LeftHand->SetSphereRadius(10.f);
	LeftHand->bGenerateOverlapEvents = true;
	//LeftHand->OnComponentBeginOverlap.AddDynamic(this, &APlaygroundHeroesCharacter::OnOverlapBegin);*/

	InputDirection = FVector(0.f, 0.f, 0.f);

	Health = 100.f;
	Stamina = 100.f;
	AttackCost = 30.f;

	StaminaGen = 17.5f;

	bIsLocked = false;
	bInputtingAttack = false;
	bAttacking = false;
	bInputtingDodge = false;
	bDodging = false;
	TimeSinceLastInput = -1.f;
	InputQueueTime = .3f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void APlaygroundHeroesCharacter::Tick(float DeltaTime)
{
	if (TimeSinceLastInput >= InputQueueTime)
	{
		bInputtingAttack = false;
		bInputtingDodge = false;
		TimeSinceLastInput = -1.f;
	}
	else if (TimeSinceLastInput >= 0.f)
	{
		TimeSinceLastInput += DeltaTime;
	}

	if (bIsLocked)
	{
		LockCameraHelper();
	}

	if (bDodging)
	{
		FVector NewLocation = UKismetMathLibrary::VLerp(GetActorLocation(), DodgeLocation, .043);
		SetActorLocation(NewLocation, true);
	}

	if (!bDodging && !bAttacking)
		Stamina = FMath::Clamp(Stamina + StaminaGen * DeltaTime, -50.f, 100.f);
}


void APlaygroundHeroesCharacter::OnHit(AJEnemy* Enemy)
{
	if (bAttacking)
	{
		if (Enemy && !Enemy->IsPendingKill())
		{
			Enemy->AddHealth(-20.f);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void APlaygroundHeroesCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("LockCam", IE_Pressed, this, &APlaygroundHeroesCharacter::LockCamera);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlaygroundHeroesCharacter::Attack);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &APlaygroundHeroesCharacter::Dodge);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlaygroundHeroesCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlaygroundHeroesCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlaygroundHeroesCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlaygroundHeroesCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &APlaygroundHeroesCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &APlaygroundHeroesCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &APlaygroundHeroesCharacter::OnResetVR);
}


void APlaygroundHeroesCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void APlaygroundHeroesCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void APlaygroundHeroesCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void APlaygroundHeroesCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	if (!bIsLocked)
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlaygroundHeroesCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	if (!bIsLocked)
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlaygroundHeroesCharacter::MoveForward(float Value)
{
	InputDirection.X = Value;
	if ((Controller != NULL) && (Value != 0.0f) && !bAttacking && !bDodging)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlaygroundHeroesCharacter::MoveRight(float Value)
{
	InputDirection.Y = Value;
	if ((Controller != NULL) && (Value != 0.0f) && !bAttacking && !bDodging)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APlaygroundHeroesCharacter::Attack()
{
	bInputtingAttack = true;
	TimeSinceLastInput = 0.f;

	if (bInputtingDodge)
	{
		bInputtingDodge = false;
	}

	AttackHelper();
}

void APlaygroundHeroesCharacter::AttackHelper()
{
	if (!bDodging && Stamina >= 0.f)
	{
		bInputtingAttack = false;
		TimeSinceLastInput = -1.f;
		Stamina = FMath::Clamp(Stamina - AttackCost, -50.f, 100.f);
		bAttacking = true;

		const FRotator Rotation = Controller->GetControlRotation();
		FRotator PlayerRotation = GetActorRotation();

		PlayerRotation.Yaw = Rotation.Yaw;

		SetActorRotation(PlayerRotation);
	}
}

void APlaygroundHeroesCharacter::Dodge()
{
	bInputtingDodge = true;
	TimeSinceLastInput = 0.f;

	if (bInputtingAttack)
	{
		bInputtingAttack = false;
	}

	DodgeHelper();
}

void APlaygroundHeroesCharacter::DodgeHelper()
{
	if (!bAttacking && Stamina >= 0.f)
	{
		bInputtingDodge = false;
		TimeSinceLastInput = -1.f;
		// Subtract Stamina
		Stamina = FMath::Clamp(Stamina - 20.f, -50.f, 100.f);
		bDodging = true;

		// If the character is currently inputting a direction
		if (InputDirection.Size() > .1f)
		{
			// Get the controller's (camera's) yaw rotation
			FRotator Rotation = Controller->GetControlRotation();
			Rotation = FRotator(0.f, Rotation.Yaw, 0.f);

			// Rotate InputDirection according to camera's rotation
			FVector Direction = Rotation.RotateVector(InputDirection);
			Direction.Normalize();

			// The location the dodge will end at is the current location + the dodge direction * 300
			DodgeLocation = GetActorLocation() + (Direction * 300);
		}
		else // If the player isn't inputting, dodge back
		{
			DodgeLocation = GetActorLocation() + GetActorForwardVector() * (-300);
		}
	}
}

void APlaygroundHeroesCharacter::LockCamera()
{
	if (bIsLocked)
	{
		bIsLocked = false;
		lockTarget = nullptr;
	}
	else
	{
		//uncomment when AJEnemy is in
		/*for (TActorIterator<AActor> itr(GetWorld()); itr; ++itr)
		{
			AJEnemy* const JEnemyItr = Cast<AJEnemy>(*itr);
			if (JEnemyItr)
			{
				if (lockTarget == nullptr)
				{
					lockTarget = JEnemyItr;
				}
				bIsLocked = true;
			}
		}*/
	}
}

void APlaygroundHeroesCharacter::LockCameraHelper()
{
	FVector enLocation = lockTarget->GetActorLocation();
	FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), enLocation);
	FRotator oldRotation = GetControlRotation();

	newRotation.Pitch = -15.f;
	newRotation.Roll = 0;

	FRotator change = UKismetMathLibrary::RLerp(oldRotation, newRotation, LockCamRate, true);

	float changeNeeded = (oldRotation.Yaw - newRotation.Clamp().Yaw) * (-1.f);
	//float changeNeeded2 = (oldRotation.Yaw - (newRotation.Clamp().Yaw - 360.f)) * (-1.f);
	//UE_LOG(LogClass, Log, TEXT("oldYaw: %f, newYaw: %f, changeNeeded: %f, changeNeeded2: %f"), oldRotation.Yaw, newRotation.Clamp().Yaw, changeNeeded, changeNeeded2);

	//AddControllerYawInput(LockCamRate * change.Yaw * GetWorld()->GetDeltaSeconds());

	Controller->SetControlRotation(change);
}
