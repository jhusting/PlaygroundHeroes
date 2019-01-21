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

	TSubclassOf<class AActor> ArrowBP;

	void ReleaseAttack();

	virtual void Tick(float DeltaTime) override;
public:
	AJArcher();
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for forwards/backward input */
	virtual void MoveForward(float Value);

	/** Called for side to side input */
	virtual void MoveRight(float Value);

	// Helper for LockCamera function, only runs when the camera can be locked onto an actor
	virtual void LockCameraHelper();

	void Attack() override;
};
