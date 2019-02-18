// Fill out your copyright notice in the Description page of Project Settings.

#include "JKnight.h"
#include "Components/InputComponent.h"

AJKnight::AJKnight()
{
	//Super::AJHero();

	bCanDamage = false;
	bBlocking = false;
}

void AJKnight::AddHealth(float Change)
{
	if (!(bDodging || bBlocking))
		Health = FMath::Clamp(Health + Change, 0.f, MaxHealth);
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

void AJKnight::Block()
{
	if(!bDodging && !bAttacking)
		bBlocking = true;
}

void AJKnight::BlockReleased()
{
	if(bBlocking)
		bBlocking = false;
}