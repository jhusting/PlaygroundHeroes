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
		TArray<FString> dialogueLines;
	//TArray<FText> dialogueLines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<float> dialogueDurations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool dialougeFinished;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool renderWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool paused;

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
		void putLinesAndDurationsInTArrays(TArray<FString> lines, TArray<float> durations);

	UFUNCTION(BlueprintCallable)
		void updateDialogue(float DeltaTime);

};
