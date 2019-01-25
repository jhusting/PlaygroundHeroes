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

	//clearing out old dialogue and durations
	dialogueLines.Empty();
	dialogueDurations.Empty();

	//refill with new dialogue and durations
	dialogueLines.Append(lines);
	dialogueDurations.Append(durations);

	//Set default values for new dialouge sequence
	timeSinceLastLineChange = 0.0f;
	lineNumber = 0;
	paused = false;
	dialougeFinished = false;
	renderWidget = true;

	//sampleText is the text that is sent to the Widget. Sets sampleText to first lnie of Dialogue
	if (dialogueLines.IsValidIndex(lineNumber)) {
		sampleText = dialogueLines[lineNumber];
		UE_LOG(LogTemp, Warning, TEXT("%s"), *sampleText);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Index %d is invalid"), lineNumber);
	}

}

void ADialogueHandler::updateDialogue(float DeltaTime) {
	if (paused) return; //Keep updateDialogue from running if paused.
	
	//Check if there are still more dialogue lines
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

		//check if duration for current line is finished
		if (timeSinceLastLineChange >= delayFor) {
			lineNumber++;

			//Set sampleText to the next line
			if (dialogueLines.IsValidIndex(lineNumber)) {
				sampleText = dialogueLines[lineNumber];
				UE_LOG(LogTemp, Warning, TEXT("%s"), *sampleText);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("dialogueLines[%d] is not a valid index"), lineNumber);
			}
			//reset time counter
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

