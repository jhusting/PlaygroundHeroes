// Fill out your copyright notice in the Description page of Project Settings.

#include "JTutorial.h"
#include <EngineGlobals.h>
#include <Runtime/Core/Public/Containers/Array.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>


// Sets default values
AJTutorial::AJTutorial()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJTutorial::BeginPlay()
{
	Super::BeginPlay();
	//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AJKnight::StaticClass(), KnightActor);
	//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AJArcher::StaticClass(), ArcherActor);
	for (TActorIterator<AActor> itr(GetWorld()); itr; ++itr)
	{
		AJKnight* const KnightItr = Cast<AJKnight>(*itr);

		if (KnightItr)
		{
			//KnightItr is your reference
			UE_LOG(LogTemp, Warning, TEXT("Knight found"));
			knight = KnightItr;
		}

		AJArcher* const ArcherItr = Cast<AJArcher>(*itr);

		if (ArcherItr)
		{
			//KnightItr is your reference
			UE_LOG(LogTemp, Warning, TEXT("Knight found"));
			archer = ArcherItr;
		}

		ADialogueHandler* DialogueHandlerItr = Cast<ADialogueHandler>(*itr);

		if (DialogueHandlerItr)
		{
			//KnightItr is your reference
			DialogueHandler = DialogueHandlerItr;
			UE_LOG(LogTemp, Warning, TEXT("DialogueHandler found"));
		}
	}
	
	
}

void AJTutorial::DisplayText() {
	TArray<FString> text;
	TArray<float> durations;

	switch (Step)
	{
	case 0:
		if (!dialogueSent) {
			text = { "Text 1", "Text 2", "Text 3" };
			durations = { 2.0f, 2.0f, 1.0f };
			DialogueHandler->sendNewDialougeSequence(text, durations);
			dialogueSent = true;			
		}
		break;

	case 1:
		if (!dialogueSent) {
			text = { "next 1", "next 2", "next 3" };
			durations = { 1.0f, 1.0f, 1.0f };
			DialogueHandler->sendNewDialougeSequence(text, durations);
			dialogueSent = true;			
		}
		break;

	case 2:
		if (!dialogueSent) {
			text = { "mext 1", "mext 2", "mext 3" };
			durations = { 2.0f, 2.0f, 2.0f };
			DialogueHandler->sendNewDialougeSequence(text, durations);
			dialogueSent = true;
			
		}
		break;

	case 3:
		if (!dialogueSent) {
			text = { "pext 1", "pext 2", "pext 3" };
			durations = { 3.0f, 2.0f, 5.0f };			
			DialogueHandler->sendNewDialougeSequence(text, durations);
			dialogueSent = true;
			
		}
		break;

	case 4:
		if (!dialogueSent) {
			text = { "sext 1", "sext 2", "sext 3" };
			durations = { 3.0f, 2.0f, 5.0f };
			DialogueHandler->sendNewDialougeSequence(text, durations);
			dialogueSent = true;
		}
		break;

	default:
		break;
	}
}

void AJTutorial::KnightMoved() {
	UE_LOG(LogTemp, Warning, TEXT("Knight Moved"));
}

void AJTutorial::KnightLockOn() {
	UE_LOG(LogTemp, Warning, TEXT("Knight Lock on"));
}

void AJTutorial::KnightAttackPressed() {

}

// Called every frame
void AJTutorial::Tick(float DeltaTime)
{
	/*
	if (archer->GetInputDirection().X > 0) {
		UE_LOG(LogTemp, Warning, TEXT("+Y"));
	}else if(archer->GetInputDirection().X < 0) {
		UE_LOG(LogTemp, Warning, TEXT("-Y"));
	}
	else if (archer->GetInputDirection().Y > 0) {
		UE_LOG(LogTemp, Warning, TEXT("+X"));
	}
	else if (archer->GetInputDirection().Y < 0) {
		UE_LOG(LogTemp, Warning, TEXT("-X"));
	}

	if (archer->GetInputAttack() || archer->GetAttacking()) {
		UE_LOG(LogTemp, Warning, TEXT("Attack"));
	}

	if (archer->GetInputDodge() || archer->GetDodging()) {
		UE_LOG(LogTemp, Warning, TEXT("Dodge"));
	}
	*/

//	UE_LOG(LogTemp, Warning, TEXT("test %s"), archer->GetDodging());

	Super::Tick(DeltaTime);
	
	switch (Step) {
	case 0:
		//DisplayText[text1, text2, text3] //intro text about world
		//When text is done, move onwards
		DisplayText();
		if (DialogueHandler->dialougeFinished) {
			Step++;
			dialogueSent = false;
		}
		break;
	case 1: //Any Movement
		DisplayText();
		if (DialogueHandler->dialougeFinished) {
			if (archer->GetInputDirection().X != 0 || archer->GetInputDirection().Y != 0
				|| knight->GetInputDirection().X != 0 || knight->GetInputDirection().Y != 0) {
				Step++;
				dialogueSent = false;
			}
		}
		break;
	case 2: //knight right stick and RB
		DisplayText();
		if (DialogueHandler->dialougeFinished) {
			
			if (/*knight->GetLockTarget() &&*/ (knight->GetInputAttack() || knight->GetAttacking()) 
				&& (archer->GetInputDodge() || archer->GetDodging() ) ) {
				Step++;
				dialogueSent = false;
			}
		}
		break;
	case 3:
		DisplayText();
		if (DialogueHandler->dialougeFinished) {
			if (/*archer->GetLockTarget() &&*/ (archer->GetInputAttack() || archer->GetAttacking())
				&& (knight->GetInputDodge() || knight->GetDodging() )) {
				Step++;
				dialogueSent = false;
			}
		}
		break;

	case 4:
		DisplayText();
		if (DialogueHandler->dialougeFinished) {
			if (/*archer->GetLockTarget() &&*/ (archer->GetInputAttack() || archer->GetAttacking())
				&& (knight->GetInputDodge() || knight->GetDodging() )) {
				Step++;
				dialogueSent = false;
			}
		}
		break;
	default:
		break;
	}
}
