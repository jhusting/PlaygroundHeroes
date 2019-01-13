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
	
protected:
	/*
	True when the player is currently in an attack animation
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bCanDamage;
};
