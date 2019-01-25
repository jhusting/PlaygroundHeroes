// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueHandler.h"
#include "Array.h"

// Sets default values
ADialogueHandler::ADialogueHandler()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	defaultDuration = 2.0f;

	renderWidget = false;
	paused = true;

	timeSinceLastLineChange = 0.0f;
	lineNumber = 0;
	maxLines = 0;
}

// Called when the game starts or when spawned
void ADialogueHandler::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADialogueHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADialogueHandler::sendNewDialougeSequence(TArray<FString> lines, TArray<float> durations) {
	maxLines = lines.Num();

	//clearing out old dialouge and druations
	dialogueLines.Empty();
	dialogueDurations.Empty();

	//Fill TArrays in with input arrays
	for (int i = 0; i < maxLines; i++) {
		dialogueLines.Add(lines[i]);
	}
	for (int i = 0; i < durations.Num(); i++) {
		dialogueDurations.Add(durations[i]);
	}

	//Set default values for new dialouge sequence
	timeSinceLastLineChange = 0.0f;
	lineNumber = 0;
	paused = false;
	dialougeFinished = false;
	renderWidget = true;

	for (int i = 0; i < dialogueLines.Num(); i++) {
		UE_LOG(LogTemp, Warning, TEXT("index[%d]: %s"), i, *dialogueLines[i]);
	}

	//sampleText is the text that is sent to the Widget. Sets sampleText to first lnie of Dialogue
	if (dialogueLines.IsValidIndex(lineNumber)) {
		sampleText = dialogueLines[lineNumber];
		UE_LOG(LogTemp, Warning, TEXT("%s"), *sampleText);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Index 0 is invalid???? really?"));
	}

}

void ADialogueHandler::updateDialogue(float DeltaTime) {
	if (paused) return; //Keep updateDialogue from running if paused.
	
	if (maxLines > lineNumber) {
		timeSinceLastLineChange += DeltaTime;

		float delayFor;
		//Use given dialogue duration, if there isn't one use the default
		if (dialogueDurations.IsValidIndex(lineNumber)) {
			delayFor = dialogueDurations[lineNumber];
		} 
		else {
			delayFor = defaultDuration;
		}

		if (timeSinceLastLineChange >= delayFor) {
			lineNumber++;

			if (dialogueLines.IsValidIndex(lineNumber)) {
				sampleText = dialogueLines[lineNumber];
				UE_LOG(LogTemp, Warning, TEXT("%s"), *sampleText);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("dialogueLines[%d] is not a valid index"), lineNumber);
			}
			
			timeSinceLastLineChange = 0.0f;
		}
	}
	else {
		renderWidget = false;
		paused = true;
		dialougeFinished = true;
	}
}

void ADialogueHandler::pauseDialogue() {
	paused = true;
	renderWidget = false;
}
void ADialogueHandler::resumeDialogue() {
	paused = false;
	renderWidget = true;
}
int ADialogueHandler::getLineNumber() {
	return lineNumber;
}

