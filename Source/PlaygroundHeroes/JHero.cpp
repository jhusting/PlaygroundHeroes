// Fill out your copyright notice in the Description page of Project Settings.

#include "JHero.h"
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
AJHero::AJHero(){ //2 sneaky
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

	InputDirection = FVector(0.f, 0.f, 0.f);

	//Get reference to Life alert to handle 'death'
	static ConstructorHelpers::FClassFinder<AActor> LifeAlertClass(TEXT("/Game/Blueprints/Generic/LifeAlert"));
	if (LifeAlertClass.Class != NULL) 
	{
		LifeAlert = LifeAlertClass.Class;
	}

	Health = 100.f;
	Stamina = 100.f;
	AttackCost = 30.f;

	StaminaGen = 17.5f;

	bIsLocked = false;
	bCameraInverted = false;
	bInputtingAttack = false;
	bAttacking = false;
	bInputtingDodge = false;
	bDodging = false;
	bCanDodge = true;
	bCanAttack = true;
	bCanInteract = true;
	MovementModifier = 1.0f;
	deathMovementModifier = 0.01f;
	TimeSinceLastInput = -1.f;
	InputQueueTime = .3f;
	bCanBeDamaged = true;

}

// Called when the game starts or when spawned
void AJHero::BeginPlay()
{
	Super::BeginPlay();
	
}

void AJHero::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
}

void AJHero::CppTick(float DeltaTime)
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
	else
		GetCharacterMovement()->bOrientRotationToMovement = true;

	if (bDodging)
	{
		FVector NewLocation = UKismetMathLibrary::VLerp(GetActorLocation(), DodgeLocation, .043);
		//SetActorLocation(NewLocation, true);
	}
	else
	{
		if (!bAttacking)
			Stamina = FMath::Clamp(Stamina + StaminaGen * DeltaTime, -50.f, MaxStamina);
		else
			Stamina = FMath::Clamp(Stamina + (StaminaGen / 4) * DeltaTime, -50.f, MaxStamina);
	}

	if (!bHasFallen) 
	{
		if (Health <= 0.f) 
		{
			Die();
		}
	}
}

float AJHero::SetMaxStamina(float n)
{
	MaxStamina = FMath::Clamp(n, 5.f, 100.f);

	if (Stamina > MaxStamina)
		Stamina = MaxStamina;

	return MaxStamina;
}

// Called to bind functionality to input
void AJHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("LockCam", IE_Pressed, this, &AJHero::LockCamera);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AJHero::Attack);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AJHero::Dodge);
	//PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AJHero::InteractPressed);
	//PlayerInputComponent->BindAction("Interact", IE_Released, this, &AJHero::InteractReleased);

	PlayerInputComponent->BindAxis("MoveForward", this, &AJHero::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AJHero::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AJHero::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AJHero::LookUpAtRate);
}

void AJHero::AddHealth(float Change, float StaggerTime)
{
	if (!bDodging) {
		Health = FMath::Clamp(Health + Change, 0.f, MaxHealth);

		if (StaggerTime > 0.0f && !bStunned)
			Stagger(StaggerTime);
	}
}

void AJHero::Stagger(float StaggerTime)
{
	prevMovement = 1.0;
	bAttacking = false;
	bDodging = false;

	if (!bStaggered) prevMovement = MovementModifier;
	if(!bHasFallen && !bStunned) MovementModifier = 0.0;
	bCanDodge = false;
	bCanAttack = false;
	bCanInteract = false;
	bStaggered = true;

	float playrate = StaggerMontageDuration / StaggerTime;
	PlayAnimMontage(StaggerMontage, playrate, FName(TEXT("Default")) );
	//GetWorldTimerManager().SetTimer(StaggerTHandle, this, &AJHero::UnStagger, StaggerTime, false);
}

void AJHero::UnStagger()
{
	if (!bHasFallen && !bStunned)
	{
		MovementModifier = prevMovement;
		bCanDodge = true;
		bCanAttack = true;
		bCanInteract = true;
		bStaggered = false;
	}
}

void AJHero::TestFunction() 
{
	UE_LOG(LogClass, Warning, TEXT("Test Function"));
}

void AJHero::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	if (!bIsLocked && !bHasFallen)
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AJHero::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	if (!bIsLocked && !bHasFallen)
	{
		if(!bCameraInverted)
			AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
		else
			AddControllerPitchInput(-1.f * Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void AJHero::MoveForward(float Value)
{
	if (!bHasFallen) 
	{
		// I know this is wrong but if I flip it it breaks so leave it
		InputDirection.X = Value;
		Value *= MovementModifier;
		if ((Controller != NULL) && (Value != 0.0f) && !bAttacking && !bDodging)
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
			//GetCharacterMovement()->AddForce(Direction*Value);
		}
	}

}

void AJHero::MoveRight(float Value)
{
	if (!bHasFallen) 
	{
		// I know this is wrong but if I flip it it breaks so leave it
		InputDirection.Y = Value;
		Value *= MovementModifier;
		if ((Controller != NULL) && (Value != 0.0f) && !bAttacking && !bDodging)
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
}

void AJHero::ResetInputBools()
{
	bAttacking = false;
	bDodging = false;
}

void AJHero::Attack()
{
	if (bCanAttack) 
	{
		bInputtingAttack = true;
		TimeSinceLastInput = 0.f;

		if (bInputtingDodge)
		{
			bInputtingDodge = false;
		}

		if (!bAttacking)
			AttackHelper();
	}
}

bool AJHero::AttackHelper()
{
	if (!bDodging && Stamina >= 0.f)
	{
		bInputtingAttack = false;
		TimeSinceLastInput = -1.f;
		Stamina = FMath::Clamp(Stamina - AttackCost, -50.f, 100.f);
		bAttacking = true;

		/*const FRotator Rotation = Controller->GetControlRotation();
		FRotator PlayerRotation = GetActorRotation();

		PlayerRotation.Yaw = Rotation.Yaw;

		SetActorRotation(PlayerRotation);*/
		return true;
	}

	return false;
}

void AJHero::Dodge()
{
	bInputtingDodge = true;
	TimeSinceLastInput = 0.f;
	
	if (bInputtingAttack)
	{
		bInputtingAttack = false;
	}
		
	if (!bDodging && bCanDodge) 
	{
		DodgeHelper();
	}
}

void AJHero::InteractPressed()
{
	if (bCanInteract)
	{
		interacting = true;
		UE_LOG(LogClass, Warning, TEXT("yes"));
	}
}

void AJHero::InteractReleased() 
{
	interacting = false;
	UE_LOG(LogClass, Warning, TEXT("no"));

}

bool AJHero::DodgeHelper()
{
	if (!bDodging && !bAttacking && Stamina >= 0.f)
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

		return true;
	}

	return false;
}

void AJHero::LockCamera()
{
	if (bIsLocked)
	{
		bIsLocked = false;
		lockTarget = nullptr;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		//uncomment when AJEnemy is in
		AJEnemy* closest = nullptr;
		float closestAngle = 360.f;
		for (TActorIterator<AActor> itr(GetWorld()); itr; ++itr)
		{
			AJEnemy* const JEnemyItr = Cast<AJEnemy>(*itr);

			if (JEnemyItr)
			{
				FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), JEnemyItr->GetActorLocation()).Clamp();
				//FRotator camRotation = GetControlRotation().Clamp();
				FRotator camRotation = GetControlRotation().Clamp();

				float angDiff = UKismetMathLibrary::NormalizedDeltaRotator(newRotation, camRotation).Yaw;
				angDiff = UKismetMathLibrary::Abs(angDiff);

				float emAngle = UKismetMathLibrary::Abs(camRotation.Yaw - newRotation.Yaw);
				
				if (angDiff < closestAngle)
				{
					closestAngle = angDiff;
					closest = JEnemyItr;
				}
			}
		}

		if (closest != nullptr)
		{
			lockTarget = closest;
			bIsLocked = true;
		}
	}
}

void AJHero::LockCameraHelper()
{
	FVector enLocation = lockTarget->GetActorLocation();
	FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), enLocation);
	FRotator oldRotation = GetControlRotation();

	newRotation.Pitch = -15.f;
	newRotation.Roll = 0;

	FRotator change = UKismetMathLibrary::RLerp(oldRotation, newRotation, LockCamRate, true);

	Controller->SetControlRotation(change);

	const FRotator Rotation = Controller->GetControlRotation();
	FRotator PlayerRotation = GetActorRotation();

	if (!bDodging)
		PlayerRotation.Yaw = Rotation.Yaw;

	SetActorRotation(PlayerRotation);
}

void AJHero::Stun() 
{
	if(!bHasFallen)
	{
		bAttacking = false;
		bDodging = false;

		bStunned = true;
		MovementModifier = 0.0;
		bCanDodge = false;
		bCanAttack = false;
		bCanInteract = false;
		bStaggered = false;
	}
}

void AJHero::Unstun()
{
	if (!bHasFallen) {
		bStunned = false;
		MovementModifier = 1.0;
		bCanDodge = true;
		bCanAttack = true;
		bCanInteract = true;
	}
}

void AJHero::Die() 
{
	UE_LOG(LogTemp, Warning, TEXT("Running Die!"));
	UWorld* const World = GetWorld();
	if (World) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Should be spawning lifeAlert"));
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = this;
		SpawnParams.Owner = this;
		AActor* lfAlert = World->SpawnActor<AActor>(LifeAlert, GetActorLocation(), GetActorRotation(), SpawnParams);

		if (lfAlert) 
		{
			mLifeAlert = lfAlert;
		}
	}
	bAttacking = false;
	bDodging = false;
	bIsLocked = false;
	
	bHasFallen = true;
	bStunned = false;
	bStaggered = false;
	bCanDodge = false;
	bCanAttack = false;
	bCanInteract = false;
	MovementModifier = deathMovementModifier;
}

void AJHero::Revive()
{
	bHasFallen = false;
	bCanDodge = true;
	bCanAttack = true;
	bCanInteract = true;
	MovementModifier = 1.0f;
	Health = MaxHealth;
	mLifeAlert->Destroy();
}

void AJHero::OrientToControlRot()
{
	const FRotator Rotation = Controller->GetControlRotation();
	FRotator PlayerRotation = GetActorRotation();

	PlayerRotation.Yaw = Rotation.Yaw;

	SetActorRotation(PlayerRotation);
}