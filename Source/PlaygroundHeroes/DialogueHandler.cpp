	// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueHandler.h"
#include "Array.h"

// Sets default values
ADialogueHandler::ADialogueHandler()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	defaultDuration = 4.0f;
	defaultFont = 0;

	renderWidget = false;
	paused = true;
	splitMode = false;
	lastSendWasInput = false;

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

/*
	Sends new sequence of Dialouge to handler. Each element of lines will be displayed for the corresponding duration and font type
	Font types: 0 = PlaygroundHeroes, 1 = OlderSister, 2 = YoungerSister
	If durations or fonts is shorter than lines it will continue to use the last lines duration or font.
	If durations or fonts are empty will use defaults of 4.0 seconds and Playgroundheroes fonts
*/
void ADialogueHandler::sendNewdialogueSequence(TArray<FString> lines, TArray<float> durations, TArray<int> fonts, TArray<FString> characters) {
	if (lastSendWasInput) { 
		splitMode = false;
		lastSendWasInput = false;
	}

	if (lines.Num() <= 0) {
		UE_LOG(LogTemp, Error, TEXT("Gave empty TArray<FString> to sendNewDialogueSequence"));
		return;
	}
	maxLines = lines.Num();

	//clearing out old dialogue and durations
	dialogueLines.Empty();
	dialogueDurations.Empty();
	fontTypes.Empty();
	speakingCharacters.Empty();

	//refill with new dialogue and durations
	dialogueLines.Append(lines);
	dialogueDurations.Append(durations);
	fontTypes.Append(fonts);
	speakingCharacters.Append(characters);

	//Set default values for new dialogue sequence
	defaultDuration = 4.0;
	defaultFont = 0;

	timeSinceLastLineChange = 0.0f;
	lineNumber = 0;
	paused = false;
	dialogueFinished = false;
	renderWidget = true;

	//centerText or player1&2 text are the text that is sent to the Widget. Sets text to first lines of dialogue
	setNewText();
}

void ADialogueHandler::sendInputText(TArray<FString> inputs, TArray<int> fonts, TArray<FString> characters) {
	lastSendWasInput = false;
	toggleSplitMode(true);
	TArray<float> dummyDurations;
	sendNewdialogueSequence(inputs, dummyDurations, fonts, characters);
	pause();
	dialogueFinished = false;
	lastSendWasInput = true;
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
			defaultDuration = dialogueDurations[lineNumber];
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
		centerText = "";
		player1Text = "";
		player2Text = "";
	}
}

void ADialogueHandler::setNewText() {
	if (splitMode) {
		centerText = "";
		centerName = "";
		if (dialogueLines.IsValidIndex(lineNumber) && dialogueLines.IsValidIndex(lineNumber + 1)) {
			player1Text = dialogueLines[lineNumber];
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *player1Text);
			if (fontTypes.IsValidIndex(lineNumber)) {
				player1Font = fontTypes[lineNumber];
			}
			else player1Font = defaultFont;

			if (speakingCharacters.IsValidIndex(lineNumber)) {
				player1Name = speakingCharacters[lineNumber];
			}
			else player1Name = "";

			lineNumber++;
			player2Text = dialogueLines[lineNumber];
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *player2Text);
			if (fontTypes.IsValidIndex(lineNumber)) {
				player2Font = fontTypes[lineNumber];
			}
			else player2Font = defaultFont;

			if (speakingCharacters.IsValidIndex(lineNumber)) {
				player2Name = speakingCharacters[lineNumber];
			}
			else player2Name = "";
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("dialogueLines[%d] or dialogueLines[%d] is not a valid index"), lineNumber - 1, lineNumber);
		}
	}
	else {
		player1Text = "";
		player2Text = "";
		player1Name = "";
		player2Name = "";
		if (dialogueLines.IsValidIndex(lineNumber)) {
			centerText = dialogueLines[lineNumber];
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *centerText);

			if (fontTypes.IsValidIndex(lineNumber)) {
				centerFont = fontTypes[lineNumber];
			}
			else centerFont = defaultFont;

			if (speakingCharacters.IsValidIndex(lineNumber)) {
				centerName = speakingCharacters[lineNumber];
			}
			else centerName = "";

		}
		else {
			UE_LOG(LogTemp, Error, TEXT("dialogueLines[%d] is not a valid index"), lineNumber);
		}
	}
	//reset time counter
	timeSinceLastLineChange = 0.0f;
}

void ADialogueHandler::saveDialogue() {
	savedLines.Empty();
	savedDurations.Empty();
	savedFonts.Empty();
	savedCharacters.Empty();

	savedLines.Append(dialogueLines);
	savedDurations.Append(dialogueDurations);
	savedFonts.Append(fontTypes);
	savedCharacters.Append(speakingCharacters);

	savedMaxLines = maxLines;
	savedSplitMode = splitMode;
	savedWasInput = lastSendWasInput;
	if (savedWasInput) {
		savedLineNumber = 0;
	}
	else {
		savedLineNumber = lineNumber;
	}
}

void ADialogueHandler::loadSavedDialogue() {
	dialogueLines.Empty();
	dialogueDurations.Empty();
	fontTypes.Empty();
	speakingCharacters.Empty();

	dialogueLines.Append(savedLines);
	dialogueDurations.Append(savedDurations);
	fontTypes.Append(savedFonts);
	speakingCharacters.Append(savedCharacters);

	lineNumber = savedLineNumber;
	maxLines = savedMaxLines;
	splitMode = savedSplitMode;

	dialogueFinished = false;
	renderWidget = true;

	if (savedWasInput) {
		paused = true;
		lastSendWasInput = true;
	}
	else {
		paused = false;
		lastSendWasInput = false;
	}

	setNewText();
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

void ADialogueHandler::clear() {
	centerText = "";
	player1Text = "";
	player2Text = "";
	centerName = "";
	player1Name = "";
	player2Name = "";
	renderWidget = false;
	dialogueFinished = true;
	paused = true;
}
