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
	splitMode = false;

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

void ADialogueHandler::sendNewdialogueSequence(TArray<FString> lines, TArray<float> durations) {
	if (lines.Num() <= 0) {
		UE_LOG(LogTemp, Error, TEXT("CUSTOM: gave empty TArray to sendNewDialogueSequence"));
		return;
	}
	maxLines = lines.Num();

	//clearing out old dialogue and durations
	dialogueLines.Empty();
	dialogueDurations.Empty();

	//refill with new dialogue and durations
	dialogueLines.Append(lines);
	dialogueDurations.Append(durations);

	//Set default values for new dialogue sequence
	timeSinceLastLineChange = 0.0f;
	lineNumber = 0;
	paused = false;
	dialogueFinished = false;
	renderWidget = true;

	//centerText or player1&2 text are the text that is sent to the Widget. Sets text to first lines of dialogue
	if (splitMode) {
		centerText = " ";
		if (dialogueLines.IsValidIndex(lineNumber) && dialogueLines.IsValidIndex(lineNumber + 1)) {
			player1Text = dialogueLines[lineNumber];
			UE_LOG(LogTemp, Warning, TEXT("%s"), *player1Text);

			lineNumber++;
			player2Text = dialogueLines[lineNumber];
			UE_LOG(LogTemp, Warning, TEXT("%s"), *player2Text);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("dialogueLines[%d] or dialogueLines[%d] is not a valid index"), lineNumber - 1, lineNumber);
		}
	}
	else {
		player1Text = " ";
		player2Text = " ";
		if (dialogueLines.IsValidIndex(lineNumber)) {
			centerText = dialogueLines[lineNumber];
			UE_LOG(LogTemp, Warning, TEXT("%s"), *centerText);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("dialogueLines[%d] is not a valid index"), lineNumber);
		}
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
			setNewText();
		}
	}
	else {
		renderWidget = false;
		paused = true;
		dialogueFinished = true;
	}
}

void ADialogueHandler::setNewText() {
	UE_LOG(LogTemp, Warning, TEXT("lineNumber = %d"), lineNumber);
	if (splitMode) {
		//make centerText blank
		centerText = " ";

		if (dialogueLines.IsValidIndex(lineNumber) && dialogueLines.IsValidIndex(lineNumber + 1)) {
			player1Text = dialogueLines[lineNumber];
			UE_LOG(LogTemp, Warning, TEXT("%s"), *player1Text);

			lineNumber++;
			player2Text = dialogueLines[lineNumber];
			UE_LOG(LogTemp, Warning, TEXT("%s"), *player2Text);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("dialogueLines[%d] or dialogueLines[%d] is not a valid index"), lineNumber - 1, lineNumber);
		}
	}
	else {
		//Make player specific text blank
		player1Text = " ";
		player2Text = " ";
		if (dialogueLines.IsValidIndex(lineNumber)) {
			centerText = dialogueLines[lineNumber];
			UE_LOG(LogTemp, Warning, TEXT("%s"), *centerText);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("dialogueLines[%d] is not a valid index"), lineNumber);
		}
	}
	//reset time counter
	timeSinceLastLineChange = 0.0f;

}

void ADialogueHandler::pause() {
	paused = true;
}
void ADialogueHandler::resume() {
	paused = false;
}
int ADialogueHandler::getLineNumber() {
	return lineNumber;
}
void  ADialogueHandler::goToNext() {
	if (splitMode) {
		lineNumber += 2; 
	}
	else {
		lineNumber++;
	}
}
void ADialogueHandler::goToLine(int newLineNumber) {
	lineNumber = newLineNumber;
}

void ADialogueHandler::toggleSplitMode(bool enabled) {
	splitMode = enabled;
}

