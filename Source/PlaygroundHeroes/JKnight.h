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
	virtual void CppTick(float DeltaTime);
	
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

	/*
		The grace period (in seconds) for a perfect block. If an attack hits the knight
		and the knight has held block for less than this time, it is a perfect block
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float PerfectBlockTime;

	/*
		This timer handle keeps track of the "perfect block" mechanic. If this handle is active,
		that means block was pressed less than PerfectBlockTime seconds ago
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	FTimerHandle PerfectBlockTHandle;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void ResetInputBools();

	virtual void Attack();
	void Block();
	void BlockReleased();
	void Dodge();

	UFUNCTION()
	void PerfectBlockEnd();

	UFUNCTION(BlueprintNativeEvent)
	void BlockPart();
	virtual void BlockPart_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void PerfectBlockPart();
	virtual void PerfectBlockPart_Implementation();
};
