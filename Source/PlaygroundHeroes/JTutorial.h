// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JArcher.h"
#include "JKnight.h"
#include "DialogueHandler.h"
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
	ADialogueHandler* DialogueHandler;

	int Step;

	//If false, not even the correct input will advance to the next stage. Intented for use to make sure any events (dialog, cutscene, etc) are fully done, before allowing continuation
	bool canAdvance;
	bool dialogueSent;
	bool inputSent;
	bool inputReady;
	bool KnightDidInput;
	bool ArcherDidInput;

	void DisplayText();

	void sendInputText(TArray<FString> inputs, TArray<int> fonts);
	void postInput();

	//Hold sequences of dialouge (switch after each input listener runs)
	TArray<FString> lines0;
	TArray<float> durations0;
	TArray<int> fonts0;

	TArray<FString> lines1;
	TArray<float> durations1;
	TArray<int> fonts1;

	TArray<FString> lines2;
	TArray<float> durations2;
	TArray<int> fonts2;

	TArray<FString> lines3;
	TArray<float> durations3;
	TArray<int> fonts3;

	TArray<FString> lines4;
	TArray<float> durations4;
	TArray<int> fonts4;

	TArray<FString> lines5;
	TArray<float> durations5;
	TArray<int> fonts5;

	TArray<FString> lines6;
	TArray<float> durations6;
	TArray<int> fonts6;

	TArray<FString> inputs0;
	TArray<FString> inputs1;
	TArray<FString> inputs2;
	TArray<FString> inputs3;
	TArray<FString> inputs4;
	TArray<int> inputFonts;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void KnightLockOn();

	void KnightAttackPressed();

	void KnightMoved();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

