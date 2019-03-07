// Fill out your copyright notice in the Description page of Project Settings.

#include "JArcher.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "JEnemy.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"


AJArcher::AJArcher()
{
	/*static ConstructorHelpers::FClassFinder<AActor> ArrowBPClass(TEXT("/Game/Blueprints/Warriors/ArrowBP"));
	if (ArrowBPClass.Class != NULL)
	{
		ArrowBP = ArrowBPClass.Class;
	}*/

	MaxHealth = 100;
	Health = MaxHealth;
	MaxStamina = 100;
	Stamina = MaxStamina;
	BaseArrowSpeed = 2500.f;
	HeldSpeedAdded = 3500.f;
	HoldTimeNeeded = 1.5f;
	minimumHoldTime = 0.3f;
}

void AJArcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJArcher::CppTick(float DeltaTime)
{
	if (timeHeld >= 0.f)
		timeHeld += DeltaTime;

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
		OrientToControlRot();
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

	if (!bHasFallen) {
		if (Health <= 0.f) {
			Die();
		}
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
	Value *= MovementModifier;
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
	Value *= MovementModifier;
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
	if (Stamina > 0.f && bCanAttack && !bDodging)
	{
		if (bInputtingDodge) 
		{
			bInputtingDodge = false;
		}

		bAttacking = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		timeHeld = 0.f;

		UWorld* const World = GetWorld();
		if (World) 
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Instigator = this;
			AActor* arrow = World->SpawnActor<AActor>(ArrowBP, GetActorLocation(), GetActorRotation(), SpawnParams);

			if (arrow) 
			{
				mArrow = arrow;

				mArrow->AttachToComponent((USceneComponent *)GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName("LeftHandSocket"));

				FTransform newTrans = FTransform(FRotator(1.264323f, 94.943405f, 168.3452f), FVector(-76.527832f, 283.226013f, 864.967285f), FVector(.75f, .75f, .75f));
				mArrow->SetActorRelativeTransform(newTrans);
				mArrow->SetActorScale3D(FVector(.75f, .75f, .75f));
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
		if (mArrow)
		{
			if (timeHeld >= minimumHoldTime) 
			{
				// First detach the arrow from our socket
				mArrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false));
				// Then get it's Projectile Movement Component
				UProjectileMovementComponent* proj = (UProjectileMovementComponent*)mArrow->GetComponentByClass(UProjectileMovementComponent::StaticClass());

				// Now we do a raytrace from the camera outwards 
				FCollisionQueryParams RV_TraceParams;
				FHitResult RV_Hit(ForceInit);

				FVector Start = GetCameraBoom()->GetSocketTransform(USpringArmComponent::SocketName).GetLocation();
				FVector End = Start + (GetFollowCamera()->GetForwardVector()) * 10000;

				// This is the actual raytrace. The info of the trace is stored in RV_Hit
				GetWorld()->LineTraceSingleByChannel(RV_Hit, Start, End, ECC_Visibility);

				// Now we try to look at the hit, and rotate the arrow to point towards it
				FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(mArrow->GetActorLocation(), RV_Hit.ImpactPoint);
				if (RV_Hit.bBlockingHit)
					mArrow->SetActorRotation(lookAt);
				else
					mArrow->SetActorRotation(GetControlRotation());

				// This is the ratio of how long we've held the button. Time Held / Total time needed to hold to get to max strength
				float heldRatio = FMath::Clamp(timeHeld / HoldTimeNeeded, 0.f, 1.f);

				// Now we need to get the arrowBP's "ArrowDamage" property
				UProperty* Property = mArrow->GetClass()->FindPropertyByName("ArrowDamage");
				if (Property) // If we successfully found that property
				{
					float* currDamage = Property->ContainerPtrToValuePtr<float>(mArrow);
					if (currDamage) //If the value has been initialized
						*currDamage = 15.f + 20.f * heldRatio; // Damage = 15 + 20 * the held ratio (this would be 100% at max strength, 0% with a 1 frame hold)
				}

				proj->SetVelocityInLocalSpace(FVector(BaseArrowSpeed + HeldSpeedAdded * heldRatio, 0.f, 0.f));
				proj->bSimulationEnabled = true;

				mArrow = nullptr;
				Stamina = FMath::Clamp(Stamina - AttackCost, -50.f, 100.f);
			}
			else 
			{
				if (mArrow)
				{
					mArrow->Destroy();
					mArrow = nullptr;
				}
			}
		}
		bAttacking = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}