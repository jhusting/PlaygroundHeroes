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

	void DisplayText();

	//Hold sequences of dialouge (switch after each input listener runs)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Sequence 0")
		TArray<FString> lines0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Sequence 0")
		TArray<float> durations0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Sequence 1")
		TArray<FString> lines1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Sequence 1")
		TArray<float> durations1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Sequence 2")
		TArray<FString> lines2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Sequence 2")
		TArray<float> durations2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Sequence 3")
		TArray<FString> lines3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Sequence 3")
		TArray<float> durations3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Sequence 4")
		TArray<FString> lines4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Sequence 4")
		TArray<float> durations4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Sequence 4")
		TArray<FString> lines5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Sequence 4")
		TArray<float> durations5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "InputText")
		TArray<FString> inputs0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "InputText")
		TArray<FString> inputs1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "InputText")
		TArray<FString> inputs2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "InputText")
		TArray<FString> inputs3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "InputText")
		TArray<float> inputDurations;


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

