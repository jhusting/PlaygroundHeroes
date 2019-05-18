// Fill out your copyright notice in the Description page of Project Settings.

#include "JKnight.h"
#include "Components/InputComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

AJKnight::AJKnight()
{
	//Super::AJHero();

	bCanDamage = false;
	bBlocking = false;
	PerfectBlockTime = 0.15f;
}

void AJKnight::AddHealth(float Change, float StaggerTime)
{
	if (GetWorldTimerManager().IsTimerActive(PerfectBlockTHandle))
	{
		Stamina = FMath::Clamp(Stamina + Change / 4.f, 0.f, 100.f);
		PerfectBlockPart();
	}
	else if (bBlocking)
	{
		Stamina = FMath::Clamp(Stamina + Change / .8f, 0.f, 100.f);
		BlockPart();
		if(StaggerTime > 0.0f)
			Stagger(StaggerTime);
	}
	else if (!bDodging)
	{
		Health = FMath::Clamp(Health + Change, 0.f, MaxHealth);
		if(StaggerTime > 0.0f)
			Stagger(StaggerTime);
	}
}

void AJKnight::Stagger(float StaggerTime) {
	prevMovement = 1.0;
	bAttacking = false;
	bDodging = false;

	if (!bStaggered) prevMovement = MovementModifier;
	if (!bHasFallen && !bStunned) MovementModifier = 0.0;

	bCanDodge = false;
	bCanAttack = false;
	bCanInteract = false;
	bStaggered = true;

	float playrate;

	if (bBlocking) 
	{
		StaggerTime = StaggerTime / 2.0f;
		playrate = BlockStaggerMontageDuration / StaggerTime;
		PlayAnimMontage(BlockStaggerMontage, playrate, FName(TEXT("Default")) );
	}
	else
	{
		playrate = StaggerMontageDuration / StaggerTime;
		PlayAnimMontage(StaggerMontage, playrate, FName(TEXT("Default")) );
	}

	//GetWorldTimerManager().SetTimer(StaggerTHandle, this, &AJHero::UnStagger, StaggerTime, false);
}

void AJKnight::UnStagger() {

	if (!bHasFallen && !bStunned)
	{
		MovementModifier = prevMovement;
		bCanDodge = true;
		bCanAttack = true;
		bCanInteract = true;
		bStaggered = false;

		if (bBlockAttempted) {
			Block();
		}
		else
		{
			BlockReleased();
		}
	}
}

void AJKnight::CppTick(float DeltaTime)
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
		if (!bAttacking && !bBlocking)
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

// Called to bind functionality to input
void AJKnight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("LockCam", IE_Pressed, this, &AJKnight::LockCamera);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AJKnight::Attack);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AJKnight::Dodge);
	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AJKnight::Block);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &AJKnight::BlockReleased);

	PlayerInputComponent->BindAxis("MoveForward", this, &AJKnight::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AJKnight::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AJKnight::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AJKnight::LookUpAtRate);
}

void AJKnight::ResetInputBools()
{
	bAttacking = false;
	bDodging = false;
	bBlocking = false;
}

void AJKnight::Attack()
{
	if (!bHasFallen) 
	{
		bInputtingAttack = true;
		TimeSinceLastInput = 0.f;

		if (bInputtingDodge)
		{
			bInputtingDodge = false;
		}

		if (!bAttacking && !bDodging)
		{
			bBlocking = false;
			AttackHelper();
		}
	}
}
void AJKnight::Dodge()
{
	if (!bHasFallen && !bAttacking) 
	{
		bInputtingDodge = true;
		TimeSinceLastInput = 0.f;

		if (bInputtingAttack)
		{
			bInputtingAttack = false;
		}
		if (!bDodging && bCanDodge) 
		{
			bBlocking = false;
			DodgeHelper();
		}
	}
}

void AJKnight::Block()
{
	if (!bDodging && !bAttacking)
	{
		bBlockAttempted = true;
		if (!bStaggered) 
		{
			bBlocking = true;
			GetWorldTimerManager().SetTimer(PerfectBlockTHandle, this, &AJKnight::PerfectBlockEnd, PerfectBlockTime, false);
		}
	}
}

void AJKnight::BlockReleased()
{
	if(bBlocking)
		bBlockAttempted = false;
		if (!bStaggered) 
		{
			bBlocking = false;
		}
}

void AJKnight::PerfectBlockEnd()
{
	UE_LOG(LogClass, Warning, TEXT("Perfect End"));
}

void AJKnight::BlockPart_Implementation()
{
	UE_LOG(LogClass, Log, TEXT("Regular Block"));
}

void AJKnight::PerfectBlockPart_Implementation()
{
	UE_LOG(LogClass, Log, TEXT("PERFECT Block"));
}