// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JHero.generated.h"

UCLASS()
class PLAYGROUNDHEROES_API AJHero : public ACharacter{
	GENERATED_BODY()
	
	TSubclassOf<class AActor> LifeAlert;
	AActor* mLifeAlert = nullptr;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	// Sets default values for this character's properties
	AJHero();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float LockCamRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool interacting;

	FORCEINLINE bool GetAttacking() const { return bAttacking; }

	FORCEINLINE bool GetDodging() const { return bDodging; }

	FORCEINLINE bool GetInputAttack() const { return bInputtingAttack; }

	FORCEINLINE bool GetInputDodge() const { return bInputtingDodge; }

	FORCEINLINE FVector GetInputDirection() const {return InputDirection;}

	FORCEINLINE bool GetIsLocked() { return bIsLocked; }

	FORCEINLINE bool GetHasFallen() { return bHasFallen; }

	FORCEINLINE float GetTimeSinceLastInput() { return TimeSinceLastInput; }
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void AddHealth(float Change);

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void TestFunction();


	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void CppTick(float DeltaTime);

protected:
	// Helper for LockCamera function, only runs when the camera can be locked onto an actor
	virtual void LockCameraHelper();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	virtual void MoveForward(float Value);

	/** Called for side to side input */
	virtual void MoveRight(float Value);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void ResetInputBools();

	UFUNCTION(Category = "Combat")
	virtual void Attack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool AttackHelper();

	UFUNCTION(Category = "Combat")
	void Dodge();

	UFUNCTION(Category = "Combat")
	void InteractPressed();

	UFUNCTION(Category = "Combat")
	void InteractReleased();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool DodgeHelper();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Die();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Revive();

	UFUNCTION(BlueprintCallable, Category = "Playground")
	void OrientToControlRot();
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
		True when player is in state they can dodge
		False is under conditions where dodging is disabled
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bCanDodge;
	
	/*
		Determines if player has fallen and can't get up
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bHasFallen;

	/*
		Used to give player temporary Speed boosts/debuffs
		multiplies directional input value.
		Default value is 1.0
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MovementModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float deathMovementModifier;

	/*
		The amount of time, in seconds, since the last player input
		-1 when there has been no input in the last InputQueueTime seconds
			OR if the input has been handled
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
	float Health = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float Stamina = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MaxStamina = 100;


	/*
		The amount of stamina generated every second
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float StaminaGen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float AttackCost;

	/*
		The direction the player is currently inputting on the left stick
		Note: This value is not normalized. If the player is inputting left: x = -1.0, y = 0, z = 0
			z is always 0
			The amount is dependent on how much they are tilting. .5 would be 50% tilt. Half tilt towards NE is x = .5, y = .5
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	FVector InputDirection;

	FVector DodgeLocation;

	/*
		A pointer to the target you are currently locked on to
		NULL when you are not locked on
	*/
	class AJEnemy* lockTarget;

	// The current hit type we are on
	// Knight Move Names: KnightAttack1, KnightAttack2, KnightAttack3
	// Archer Move Names: ArcherHit1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FString CurrentHitType;
};
