#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JKid.generated.h"

UCLASS()
class PLAYGROUNDHEROES_API AJKid : public ACharacter {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

public:
	// Sets default values for this character's properties
	AJKid();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
		float LockCamRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		bool bCameraInverted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool interacting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool bCrawling;

	FORCEINLINE FVector GetInputDirection() const { return InputDirection; }

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void Tick(float DeltaTime) override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	virtual void MoveForward(float Value);

	/** Called for side to side input */
	virtual void MoveRight(float Value);

	UFUNCTION()
		virtual void InteractPressed();
	UFUNCTION()
		virtual void InteractReleased();

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

	/*
		Used to give player temporary Speed boosts/debuffs
		multiplies directional input value.
		Default value is 1.0
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float MovementModifier;

	/*
		The direction the player is currently inputting on the left stick
		Note: This value is not normalized. If the player is inputting left: x = -1.0, y = 0, z = 0
			z is always 0
			The amount is dependent on how much they are tilting. .5 would be 50% tilt. Half tilt towards NE is x = .5, y = .5
	*/
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
		FVector InputDirection;
};