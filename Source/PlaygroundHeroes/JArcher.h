// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JHero.h"
#include "JArcher.generated.h"

/**
 * 
 */
UCLASS()
class PLAYGROUNDHEROES_API AJArcher : public AJHero
{
	GENERATED_BODY()

	void ReleaseAttack();

	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintCallable)
	virtual void CppTick(float DeltaTime) override;

	AActor* mArrow = nullptr;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classes")
	TSubclassOf<class AActor> ArrowBP;

	AJArcher();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float HeldSpeedAdded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float BaseArrowSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float HoldTimeNeeded;

	UPROPERTY(BlueprintReadOnly)
	float timeHeld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float minimumHoldTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float heldSensitivity;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for forwards/backward input */
	virtual void MoveForward(float Value);

	/** Called for side to side input */
	virtual void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	virtual void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	virtual void LookUpAtRate(float Rate);

	virtual void Stagger(float StaggerTime);

	virtual void LockCamera();

	// Helper for LockCamera function, only runs when the camera can be locked onto an actor
	virtual void LockCameraHelper();

	void Attack() override;
};
