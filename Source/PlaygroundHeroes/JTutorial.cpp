 // Fill out your copyright notice in the Description page of Project Settings.

#include "JTutorial.h"
#include <EngineGlobals.h>
#include <Runtime/Core/Public/Containers/Array.h>
#include "Runtime/Engine/Classes/Engine/World.h"
#include <Runtime/Engine/Classes/Engine/Engine.h>


// Sets default values
AJTutorial::AJTutorial()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//set default dialogue lines:
	lines0.Add("ABIGAIL: *sighs* Leila I'm bored. Can you tell me a story?");	
	lines0.Add("LEILA: Well sure... uh, let's see.... Once upon a time there were two valiant heroes: Leila, and her younger sister, Abigail. "); 
	lines0.Add("LEILA: Throughout the lands they were known to be the strongest-");
	lines0.Add("ABIGAIL: No not that one, I've heard it a million times. Something new!");
	lines0.Add("LEILA: Okay, okay. Something new.... ");
	lines0.Add("LEILA: After Leila and Abigail saved the kingdom from the evil prince, they thought their fighting days were over. ");
	lines0.Add("LEILA: The kingdom was safer than ever, and it seemed that the two heroes no longer had much of a job to do. ");
	lines0.Add("LEILA: Without any threats to the kingdom, they retired from battle and spent most of their days ");
	lines0.Add("LEILA: assisting villagers throughout the kingdom. ");
	lines0.Add("LEILA: Until one day...");
	lines0.Add("LEILA: A dark wizard entered the realm. The joy and beauty he saw throughout the land disgusted him, and he vowed to destroy it.");
	lines0.Add("LEILA: His name was......");
	lines0.Add("ABIGAIL: Zorrander!");
	lines0.Add("LEILA: Yes! His name was... Zorrander.");
	lines0.Add("LEILA: Zorrander retreated to his lair in the forest, ");
	lines0.Add("LEILA: where he began brewing a curse so terrible that it would suck all happiness from the kingdom, ");
	lines0.Add("LEILA: leaving only hatred and misery. There were only two people in the land that were possibly strong enough to stop him... ");
	lines0.Add("LEILA: Leila and Abigail!");
	lines0.Add("LEILA: Now what do you say, are the two heroes brave enough to go on this quest?");
	lines0.Add("ABIGAIL: Of course they are! They have to defeat Zorrander!");
	lines0.Add("LEILA: Alright then, Abigail. We must prepare for our adventure into the forest!");

	lines1.Add("LEILA: Now, although we may be the strongest fighters in all the land, we still need to practice!");
	lines1.Add("LEILA: We can't be rusty when we battle Zorrander.");
	lines1.Add("LEILA: First let's get used to moving around in our armor again.");
	inputDurations.Add(3.0);

	inputs0.Add("<Player 1: Use left stick to move>");
	inputs0.Add("<Player 2: Use left stick to move>");

	lines2.Add("ABIGAIL: Bet you can't hit me with your sword!");
	lines2.Add("LEILA: Oh yeah? We'll see about that!");

	inputs1.Add("<Player 1: Hold the RT and push the Left Stick in a direction to Dodge>");
	inputs1.Add("<Player 2: Press the Right Stick to Lock On to a target. When in melee range press RB to Attack>");

	lines3.Add("LEILA: Nice job! I think we're almost ready for our adventure. ");
	lines3.Add("LEILA: Now try to hit me with an arrow!");
	lines3.Add("ABIGAIL: Ok, stand back!");

	inputs2.Add("<Player 1: Right Stick to Lock On to a target. Hold RB to charge an arrow, release to Shoot>");
	inputs2.Add("<Player 2: Hold the RT and push the Left Stick in a direction to Dodge>");

	lines4.Add("ABIGAIL: Even though we haven't battled in ages, it's all coming back to me now.");
	lines4.Add("ABIGAIL: Why don't you try blocking an arrow with your shield?");

	inputs3.Add("<Player 1: Hold RB to charge an arrow, release to Shoot>");
	inputs3.Add("<Player 2: Hold the LB to Block an incoming attack>");

	lines5.Add("LEILA: *huffs* Good fight.");
	lines5.Add("LEILA: We can keep training if you'd like,");
	lines5.Add("but I think we're about ready to head into the forest.");

	inputs4.Add("<Player 1: Take any action>");
	inputs4.Add("<Player 2: Take any action>");

	lines6.Add("LEILA: Now we have to be careful on our journey to find Zorrander,");
	lines6.Add("LEILA: he's going to do everything in his power to stop us from reaching him.");
	lines6.Add("LEILA: What do we know about him?");
	lines6.Add("ABIGAIL: Well, when he learned that the two strongest heroes were on their way to his lair,");
	lines6.Add("ABIGAIL: he got scared that his evil plans would be ruined! ");
	lines6.Add("ABIGAIL: So, to protect himself he created powerful monsters to guard the forest and stop us from reaching him.");
	lines6.Add("LEILA: Like that will stop us!");
	lines6.Add("ABIGAIL: Of course it won't! But he believed that only the purest evil would be strong enough to stop us... ");
	lines6.Add("ABIGAIL: so he created each monster using a piece of his black heart. The monsters are all a part of him.");
	lines6.Add("LEILA: Hmm so that must mean if we defeat the monsters Zorrander becomes weaker?");
	lines6.Add("ABIGAIL: Yes, but each monster will try their best to keep us out of the forest.");
	lines6.Add("ABIGAIL: They can see our deepest darkest fears and will use that to try and scare us away.");
	lines6.Add("LEILA: ...They know our deepest darkest fears...?");
	lines6.Add("ABIGAIL: Yup but that's ok because we're the bravest heroes!");
	lines6.Add("LEILA: You're right, they won't scare us.");
	lines6.Add("ABIGAIL: Leila, what's your greatest fear?");
	lines6.Add("LEILA: ... My greatest fear is... losing you.");
	lines6.Add("ABIGAIL: Losing me? That seems pretty lame. Don't worry, we'll always be together! Right?");
	lines6.Add("LEILA: Of course silly... ");
}

// Called when the game starts or when spawned
void AJTutorial::BeginPlay()
{
	Super::BeginPlay();
	//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AJKnight::StaticClass(), KnightActor);
	//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AJArcher::StaticClass(), ArcherActor);
	dialogueSent = false;
	inputSent = false;

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

	case 6:
		if (!dialogueSent) {
			DialogueHandler->sendNewdialogueSequence(lines6, durations6);
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
	if (!knight) {
		UE_LOG(LogTemp, Error, TEXT("NO KNIGHT WAS FOUND"));
		return;
	}
	if (!archer) { 
		UE_LOG(LogTemp, Error, TEXT("NO ARCHER WAS FOUND"));
		return; 
	}
	if (!DialogueHandler) {
		UE_LOG(LogTemp, Error, TEXT("NO DIALOGUEHANDLER WAS FOUND"));
		return;
	}

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
			sendInputText(inputs0, inputDurations);

			if (archer->GetInputDirection().X != 0 || archer->GetInputDirection().Y != 0) ArcherDidInput = true;
			if (knight->GetInputDirection().X != 0 || knight->GetInputDirection().Y != 0) KnightDidInput = true;

			if (KnightDidInput && ArcherDidInput){
				postInput();
				Step++;
				dialogueSent = false;
			}
		}
		break;
	case 2: //knight right stick and RB
		DisplayText();
		if (DialogueHandler->dialogueFinished) {

			//send input instruction text
			sendInputText(inputs1, inputDurations);
			if (knight->GetIsLocked() && (knight->GetInputAttack() || knight->GetAttacking())) KnightDidInput = true;
			if ((archer->GetInputDodge() || archer->GetDodging())) ArcherDidInput = true;

			if (KnightDidInput && ArcherDidInput) {
				postInput();
				Step++;
				dialogueSent = false;
			}
		}
		break;
	case 3:
		DisplayText();
		if (DialogueHandler->dialogueFinished) {

			//send input instruction text
			sendInputText(inputs2, inputDurations);

			if (archer->GetIsLocked() && (archer->GetInputAttack() || archer->GetAttacking())) ArcherDidInput = true;
			if (knight->GetInputDodge() || knight->GetDodging()) KnightDidInput = true;

			if (KnightDidInput && ArcherDidInput) {
				postInput();
				Step++;				
				dialogueSent = false;
			}
		}
		break;

	case 4:
		DisplayText();
		if (DialogueHandler->dialogueFinished) {

			//send input instruction text
			sendInputText(inputs3, inputDurations);

			if (archer->GetInputAttack() || archer->GetAttacking()) ArcherDidInput = true;
			if (knight->GetInputDodge() || knight->GetDodging()) KnightDidInput = true;

			if (KnightDidInput && ArcherDidInput) {
				postInput();
				Step++;	
				dialogueSent = false;
			}
		}
		break;

	case 5:
		//DisplayText[text1, text2, text3] //end of tutorial text
		//When text is done, move onwards
		DisplayText();
		if (DialogueHandler->dialogueFinished) {
			sendInputText(inputs4, inputDurations);

			if (archer->GetInputAttack() || archer->GetAttacking() || archer->GetInputDodge() || archer->GetDodging()) ArcherDidInput = true;
			if (knight->GetInputAttack() || knight->GetAttacking() || knight->GetInputDodge() || knight->GetDodging()) KnightDidInput = true;
			if (KnightDidInput && ArcherDidInput) {
				postInput();
				Step++;
				dialogueSent = false;
			}
		}
		break;

	case 6:
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

//Enacts changes in DialogueHandler for Input Prompt text and sends text
void AJTutorial::sendInputText(TArray<FString> inputs, TArray<float> inputDuration) {
	if (!inputSent) {
		DialogueHandler->toggleSplitMode(true);
		DialogueHandler->sendNewdialogueSequence(inputs, inputDuration);
		DialogueHandler->pause();
		DialogueHandler->dialogueFinished = true;
		inputSent = true;
	}
}

//Readies JTutorial for sending text after an input prompt
void AJTutorial::postInput() {
	KnightDidInput = false;
	ArcherDidInput = false;
	DialogueHandler->toggleSplitMode(false);
	inputSent = false;
}


