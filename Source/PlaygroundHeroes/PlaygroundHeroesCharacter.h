// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlaygroundHeroesCharacter.generated.h"


UCLASS(config=Game)
class APlaygroundHeroesCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	// Right hand sphere
	UPROPERTY(VisibleAnywhere, Category = "Combat", meta = (AllowPriveAccess = "true"))
	class USphereComponent* RightHand;

	// Left hand sphere
	UPROPERTY(VisibleAnywhere, Category = "Combat", meta = (AllowPriveAccess = "true"))
	class USphereComponent* LeftHand;
public:
	APlaygroundHeroesCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float LockCamRate;

	UFUNCTION(BlueprintCallable)
	void OnHit(AJEnemy* Enemy);

	virtual void Tick(float DeltaTime) override;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	UFUNCTION(Category = "Combat")
	void Attack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AttackHelper();

	UFUNCTION(Category = "Combat")
	void Dodge();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void DodgeHelper();

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// Handler for when the camera is to be locked
	void LockCamera();

	// True when the camera is locked
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "JCamera")
	bool bIsLocked;

	/*
		True when the last player input was attack
		Set to false after InputQueueTime seconds after input
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bInputtingAttack;

	/*
	True when the player is currently in an attack animation
	and is consequently invulnerable
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bAttacking;

	/*
		True when the last player input was dodge
		Set to false after InputQueueTime seconds after input
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bInputtingDodge;

	/*
	True when the player is currently in the dodge animation
	and is consequently invulnerable
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bDodging;

	/*
		The amount of time, in seconds, since the last player input
		-1 when there has been no input in the last InputQueueTime seconds
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	float TimeSinceLastInput;

	/*
		The maximum amount of time, in seconds, that we will keep
		track of player input
		default = .3f (.3 seconds)
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float InputQueueTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float StaminaGen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float AttackCost;

	FVector DodgeLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	FVector InputDirection;

	/*
		A pointer to the target you are currently locked on to
		NULL when you are not locked on
	*/
	class AJEnemy* lockTarget;

private:
	// Helper for LockCamera function, only runs when the camera can be locked onto an actor
	void LockCameraHelper();
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:

	FORCEINLINE bool GetAttacking() const { return bAttacking; }

	FORCEINLINE bool GetDodging() const { return bDodging; }

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};



