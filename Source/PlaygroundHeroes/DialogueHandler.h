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
		FString centerText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString player1Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString player2Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int player1Font;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int player2Font;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int centerFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString centerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString player1Name;

	UPROPERTY(EditAnywhere, BlueprintReadwrite)
		FString player2Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> dialogueLines;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<float> dialogueDurations;

	//0 default, 1 Oldersister, 2 younger sister
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int> fontTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> speakingCharacters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool dialogueFinished;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool renderWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool paused;

	//Toggles putting different dialogue on each players screen. Uses 2 dialogue lines.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool splitMode;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//helper function for updateDialogue
	void setNewText();

	float defaultDuration;
	int defaultFont;
	float timeSinceLastLineChange;
	int lineNumber;
	int maxLines;
	bool lastSendWasInput;

	TArray<FString> savedLines;
	TArray<float> savedDurations;
	TArray<int> savedFonts;
	TArray<FString> savedCharacters;
	int savedLineNumber;
	int savedMaxLines;
	bool savedSplitMode;
	bool savedWasInput;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Send a new Sequence of dialogue lines to interate through
	UFUNCTION(BlueprintCallable)
		void sendNewdialogueSequence(TArray<FString> lines, TArray<float> durations, TArray<int> fonts, TArray<FString> characters);

	/*Send new input prompt text. Make TArrays 2 elements long.
	 *index 0 appears for player 0, index 1 for player 1 
	 */
	UFUNCTION(BlueprintCallable)
		void sendInputText(TArray<FString> inputs, TArray<int> fontType, TArray<FString> characters);

	UFUNCTION(BlueprintCallable)
		void updateDialogue(float DeltaTime);

	UFUNCTION(BlueprintCallable)
		void saveDialogue();

	UFUNCTION(BlueprintCallable)
		void loadSavedDialogue();

	UFUNCTION(BlueprintCallable)
		void pause();
	
	UFUNCTION(BlueprintCallable)
		void resume();

	UFUNCTION(BlueprintCallable)
		int getLineNumber();

	UFUNCTION(BlueprintCallable)
		void goToNext();

	UFUNCTION(BlueprintCallable)
		void goToLine(int newLineNumber);

	//determines whether displays center text or different text for each player
	UFUNCTION(BlueprintCallable)
		void toggleSplitMode(bool enabled);

	UFUNCTION(BlueprintCallable)
		void clear();
};
