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

	//set default dialogue lines:
	lines0.Add("introText 1");	
	lines0.Add("introText 2"); 
	lines0.Add("introText 3");
	durations0.Add(3.0);

	lines1.Add("LEILA: Now, although we may be the strongest fighters in all the land, we still need to practice!");
	lines1.Add("LEILA: We can’t be rusty when we battle Zorrander.");
	lines1.Add("LEILA: First let’s get used to moving around in our armor again.");
	inputDurations.Add(3.0);

	inputs0.Add("<Player 1: Use left stick to move>");
	inputs0.Add("<Player 2: Use left stick to move>");

	lines2.Add("ABIGAIL: Bet you can’t hit me with your sword!");
	lines2.Add("LEILA: Oh yeah? We’ll see about that!");

	inputs1.Add("<Player 2: Press the Right Stick to Lock On to a target. When in melee range press RB to Attack>");
	inputs1.Add("<Player 1: Hold the RT and push the Left Stick in a direction to Dodge>");

	lines3.Add("LEILA: Nice job! I think we’re almost ready for our adventure. ");
	lines3.Add("LEILA: Now try to hit me with an arrow!");
	lines3.Add("ABIGAIL: Ok, stand back!");

	inputs2.Add("<Player 1: Right Stick to Lock On to a target. Hold RB to charge an arrow, release to Shoot>");
	inputs2.Add("<Player 2: Hold the RT and push the Left Stick in a direction to Dodge>");

	lines4.Add("ABIGAIL: Even though we haven’t battled in ages, it’s all coming back to me now.");
	lines4.Add("ABIGAIL: Why don’t you try blocking an arrow with your shield?");

	inputs3.Add("<Player 1: Hold RB to charge an arrow, release to Shoot>");
	inputs3.Add("<Player 2: Hold the LB to Block an incoming attack>");

	lines5.Add("LEILA: *huffs* Good fight.");
	lines5.Add("LEILA: We can keep training if you’d like,");
	lines5.Add("but I think we’re about ready to head into the forest.");
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

	switch (Step)
	{
	case 0:
		if (!dialogueSent) {
			DialogueHandler->sendNewdialogueSequence(lines0, durations0);
			dialogueSent = true;			
		}
		break;

	case 1:
		if (!dialogueSent) {
			DialogueHandler->sendNewdialogueSequence(lines1, durations1);
			dialogueSent = true;			
		}
		break;

	case 2:
		if (!dialogueSent) {
			DialogueHandler->sendNewdialogueSequence(lines2, durations2);
			dialogueSent = true;
			
		}
		break;

	case 3:
		if (!dialogueSent) {	
			DialogueHandler->sendNewdialogueSequence(lines3, durations3);
			dialogueSent = true;
			
		}
		break;

	case 4:
		if (!dialogueSent) {
			DialogueHandler->sendNewdialogueSequence(lines4, durations4);
			dialogueSent = true;
		}
		break;

	case 5:
		if (!dialogueSent) {
			DialogueHandler->sendNewdialogueSequence(lines5, durations5);
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
		if (DialogueHandler->dialogueFinished) {
			Step++;
			dialogueSent = false;
		}
		break;
	case 1: //Any Movement
		DisplayText();
		if (DialogueHandler->dialogueFinished) {

			//send input instruction text
			DialogueHandler->toggleSplitMode(true);
			DialogueHandler->sendNewdialogueSequence(inputs0, inputDurations);
			DialogueHandler->pause();
			DialogueHandler->dialogueFinished = true;

			if (archer->GetInputDirection().X != 0 || archer->GetInputDirection().Y != 0
				|| knight->GetInputDirection().X != 0 || knight->GetInputDirection().Y != 0) {
				Step++;
				DialogueHandler->toggleSplitMode(false);
				dialogueSent = false;
			}
		}
		break;
	case 2: //knight right stick and RB
		DisplayText();
		if (DialogueHandler->dialogueFinished) {

			//send input instruction text
			DialogueHandler->toggleSplitMode(true);
			DialogueHandler->sendNewdialogueSequence(inputs0, inputDurations);
			DialogueHandler->pause();
			DialogueHandler->dialogueFinished = true;
			
			if (/*knight->GetLockTarget() &&*/ (knight->GetInputAttack() || knight->GetAttacking()) 
				&& (archer->GetInputDodge() || archer->GetDodging() ) ) {
				Step++;
				DialogueHandler->toggleSplitMode(false);
				dialogueSent = false;
			}
		}
		break;
	case 3:
		DisplayText();
		if (DialogueHandler->dialogueFinished) {

			//send input instruction text
			DialogueHandler->toggleSplitMode(true);
			DialogueHandler->sendNewdialogueSequence(inputs0, inputDurations);
			DialogueHandler->pause();
			DialogueHandler->dialogueFinished = true;

			if (/*archer->GetLockTarget() &&*/ (archer->GetInputAttack() || archer->GetAttacking())
				&& (knight->GetInputDodge() || knight->GetDodging() )) {
				Step++;
				DialogueHandler->toggleSplitMode(false);
				dialogueSent = false;
			}
		}
		break;

	case 4:
		DisplayText();
		if (DialogueHandler->dialogueFinished) {

			//send input instruction text
			DialogueHandler->toggleSplitMode(true);
			DialogueHandler->sendNewdialogueSequence(inputs0, inputDurations);
			DialogueHandler->pause();
			DialogueHandler->dialogueFinished = true;

			if (/*archer->GetLockTarget() &&*/ (archer->GetInputAttack() || archer->GetAttacking())
				&& (knight->GetInputDodge() || knight->GetDodging() )) {
				Step++;
				DialogueHandler->toggleSplitMode(false);	
				dialogueSent = false;
			}
		}
		break;

	case 5:
		//DisplayText[text1, text2, text3] //end of tutorial text
		//When text is done, move onwards
		DisplayText();
		if (DialogueHandler->dialogueFinished) {
			Step++;
			dialogueSent = false;
		}
		break;

	default:
		break;
	}
}
