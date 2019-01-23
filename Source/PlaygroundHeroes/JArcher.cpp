// Fill out your copyright notice in the Description page of Project Settings.

#include "JArcher.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "JEnemy.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"


AJArcher::AJArcher()
{

	static ConstructorHelpers::FClassFinder<AActor> ArrowBPClass(TEXT("/Game/Blueprints/ArrowBP"));
	if (ArrowBPClass.Class != NULL)
	{
		ArrowBP = ArrowBPClass.Class;
	}
	MaxHealth = 100;
	Health = MaxHealth;
	MaxStamina = 100;
	Stamina = MaxStamina;
}

void AJArcher::Tick(float DeltaTime)
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
		Super::LockCameraHelper();
	}

	if (bAttacking)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		FRotator PlayerRotation = GetActorRotation();

		PlayerRotation.Yaw = Rotation.Yaw;

		SetActorRotation(PlayerRotation);
	}

	if (bDodging)
	{
		FVector NewLocation = UKismetMathLibrary::VLerp(GetActorLocation(), DodgeLocation, .043);
		SetActorLocation(NewLocation, true);
	}

	if (!bDodging)
	{
		if(!bAttacking)
			Stamina = FMath::Clamp(Stamina + StaminaGen * DeltaTime, -50.f, 100.f);
		else
			Stamina = FMath::Clamp(Stamina + (StaminaGen/4) * DeltaTime, -50.f, 100.f);
	}
}

// Called to bind functionality to input
void AJArcher::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("LockCam", IE_Pressed, this, &AJArcher::LockCamera);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AJArcher::Attack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AJArcher::ReleaseAttack);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AJArcher::Dodge);

	PlayerInputComponent->BindAxis("MoveForward", this, &AJArcher::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AJArcher::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AJArcher::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AJArcher::LookUpAtRate);
}

void AJArcher::MoveForward(float Value)
{
	// I know this is wrong but if I flip it it breaks so leave it
	InputDirection.X = Value;
	if ((Controller != NULL) && (Value != 0.0f) && !bDodging)
	{
		if (bAttacking)
			Value /= 4;
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		//GetCharacterMovement()->AddForce(Direction*Value);
	}
}

void AJArcher::MoveRight(float Value)
{
	// I know this is wrong but if I flip it it breaks so leave it
	InputDirection.Y = Value;
	if ((Controller != NULL) && (Value != 0.0f) && !bDodging)
	{
		if (bAttacking)
			Value /= 4;
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
void AJArcher::LockCameraHelper()
{
	FVector enLocation = lockTarget->GetActorLocation();
	FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), enLocation);
	FRotator oldRotation = GetControlRotation();

	newRotation.Pitch = -15.f;
	newRotation.Roll = 0;

	FRotator change = UKismetMathLibrary::RLerp(oldRotation, newRotation, LockCamRate, true);

	//Controller->SetControlRotation(change);

	if ((oldRotation - newRotation).Yaw < 3.f)
	{
		bIsLocked = false;
		lockTarget = nullptr;
	}
}

void AJArcher::Attack()
{
	if (Stamina > 0.f)
	{
		bAttacking = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;

		UWorld* const World = GetWorld();
		if (World) 
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Instigator = this;
			AActor* arrow = World->SpawnActor<AActor>(ArrowBP, GetActorLocation(), GetActorRotation(), SpawnParams);
			if (arrow) 
			{
				//arrow->attachrootcomp
				//arrow->AttachToComponent(GetMesh(),)
			}
		}
	}
	else
		bAttacking = false;
}

void AJArcher::ReleaseAttack()
{
	if (bAttacking)
	{
		//do stuff
		Stamina = FMath::Clamp(Stamina - AttackCost, -50.f, 100.f);
		bAttacking = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}