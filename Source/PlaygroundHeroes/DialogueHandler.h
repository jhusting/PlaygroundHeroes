// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DialogueHandler.generated.h"


using namespace std;

UCLASS()
class PLAYGROUNDHEROES_API ADialogueHandler : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADialogueHandler();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString sampleText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString player1Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString player2Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> dialogueLines;
	//TArray<FText> dialogueLines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<float> dialogueDurations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool dialogueFinished;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool renderWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool paused;

	//Toggles putting different dialogue on each players screen. Uses 2 dialogue lines.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool differentBetweenPlayers;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float defaultDuration;
	float timeSinceLastLineChange;
	int lineNumber;
	int maxLines;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void sendNewdialogueSequence(TArray<FString> lines, TArray<float> durations);

	UFUNCTION(BlueprintCallable)
		void updateDialogue(float DeltaTime);

	UFUNCTION(BlueprintCallable)
		void pauseDialogue();
	
	UFUNCTION(BlueprintCallable)
		void resumeDialogue();

	UFUNCTION(BlueprintCallable)
		int getLineNumber();
};
