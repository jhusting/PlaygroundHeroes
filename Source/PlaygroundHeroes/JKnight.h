// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JHero.h"
#include "JKnight.generated.h"

/**
 * 
 */
UCLASS()
class PLAYGROUNDHEROES_API AJKnight : public AJHero
{
	GENERATED_BODY()

public:
	AJKnight();

	FORCEINLINE bool GetBlocking() const { return bBlocking; }

	virtual void AddHealth(float Change);
	
protected:
	/*
		True when the player is currently in an attack animation, and can cause damage
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bCanDamage;

	/*
		True when the player is blocking
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bBlocking;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void Attack();
	void Block();
	void BlockReleased();
};
