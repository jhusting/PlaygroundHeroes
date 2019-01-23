// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JArcher.h"
#include "JKnight.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EngineUtils.h"
#include "JTutorial.generated.h"


UCLASS()
class PLAYGROUNDHEROES_API AJTutorial : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AJTutorial();

	AJKnight* knight;
	AJArcher* archer;

	int Step;

	void DisplayText();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void KnightLockOn();

	void KnightAttackPressed();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

