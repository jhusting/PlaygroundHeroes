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
	fonts0.Add(2);
	lines0.Add("LEILA: Well sure... uh, let's see.... Once upon a time there were two valiant heroes: Leila, and her younger sister, Abigail. "); 
	fonts0.Add(1);
	lines0.Add("LEILA: Throughout the lands they were known to be the strongest-");
	fonts0.Add(1);
	lines0.Add("ABIGAIL: No not that one, I've heard it a million times. Something new!");
	fonts0.Add(2);
	lines0.Add("LEILA: Okay, okay. Something new.... ");
	fonts0.Add(1);
	lines0.Add("LEILA: After Leila and Abigail saved the kingdom from the evil prince, they thought their fighting days were over. ");
	fonts0.Add(1);
	lines0.Add("LEILA: The kingdom was safer than ever, and it seemed that the two heroes no longer had much of a job to do. ");
	fonts0.Add(1);
	lines0.Add("LEILA: Without any threats to the kingdom, they retired from battle and spent most of their days ");
	fonts0.Add(1);
	lines0.Add("LEILA: assisting villagers throughout the kingdom. ");
	fonts0.Add(1);
	lines0.Add("LEILA: Until one day...");
	fonts0.Add(1);
	lines0.Add("LEILA: A dark wizard entered the realm. The joy and beauty he saw throughout the land disgusted him, and he vowed to destroy it.");
	fonts0.Add(1);
	lines0.Add("LEILA: His name was......");
	fonts0.Add(1);
	lines0.Add("ABIGAIL: Zorrander!");
	fonts0.Add(2);
	lines0.Add("LEILA: Yes! His name was... Zorrander.");
	fonts0.Add(1);
	lines0.Add("LEILA: Zorrander retreated to his lair in the forest, ");
	fonts0.Add(1);
	lines0.Add("LEILA: where he began brewing a curse so terrible that it would suck all happiness from the kingdom, ");
	fonts0.Add(1);
	lines0.Add("LEILA: leaving only hatred and misery. There were only two people in the land that were possibly strong enough to stop him... ");
	fonts0.Add(1);
	lines0.Add("LEILA: Leila and Abigail!");
	fonts0.Add(1);
	lines0.Add("LEILA: Now what do you say, are the two heroes brave enough to go on this quest?");
	fonts0.Add(1);
	lines0.Add("ABIGAIL: Of course they are! They have to defeat Zorrander!");
	fonts0.Add(2);
	lines0.Add("LEILA: Alright then, Abigail. We must prepare for our adventure into the forest!");
	fonts0.Add(1);

	lines1.Add("LEILA: Now, although we may be the strongest fighters in all the land, we still need to practice!");
	fonts1.Add(1);
	lines1.Add("LEILA: We can't be rusty when we battle Zorrander.");
	fonts1.Add(1);
	lines1.Add("LEILA: First let's get used to moving around in our armor again.");
	fonts1.Add(1);

	inputs0.Add("<Player 1: Use left stick to move>");
	inputs0.Add("<Player 2: Use left stick to move>");
	inputFonts.Add(0);
	inputFonts.Add(0);

	lines2.Add("ABIGAIL: Bet you can't hit me with your sword!");
	fonts2.Add(2);
	lines2.Add("LEILA: Oh yeah? We'll see about that!");
	fonts2.Add(1);

	inputs1.Add("<Player 1: Hold a direction with the left stick and press B to Dodge>");
	inputs1.Add("<Player 2: Press the Right Stick to Lock On to a target. When in melee range press RB to Attack>");

	lines3.Add("LEILA: Nice job! I think we're almost ready for our adventure. ");
	fonts3.Add(1);
	lines3.Add("LEILA: Now try to hit me with an arrow!");
	fonts3.Add(1);
	lines3.Add("ABIGAIL: Ok, stand back!");
	fonts3.Add(2);

	inputs2.Add("<Player 1: Right Stick to focus on a target. Hold RB to charge an arrow, release to Shoot>");
	inputs2.Add("<Player 2: Hold a direction with the left stick and press B to Dodge>");

	lines4.Add("ABIGAIL: Even though we haven't battled in ages, it's all coming back to me now.");
	fonts4.Add(2);
	lines4.Add("ABIGAIL: Why don't you try blocking an arrow with your shield?");
	fonts4.Add(2);

	inputs3.Add("<Player 1: Hold RB to charge an arrow, release to Shoot>");
	inputs3.Add("<Player 2: Hold the LB to Block an incoming attack>");

	lines5.Add("LEILA: *huffs* Good fight.");
	fonts5.Add(1);
	lines5.Add("LEILA: We can keep training if you'd like,");
	fonts5.Add(1);
	lines5.Add("LEILA: but I think we're about ready to head into the forest.");
	fonts5.Add(1);

	inputs4.Add("<Player 1: Take any action>");
	inputs4.Add("<Player 2: Take any action>");

	lines6.Add("LEILA: Now we have to be careful on our journey to find Zorrander,");
	fonts6.Add(1);
	lines6.Add("LEILA: he's going to do everything in his power to stop us from reaching him.");
	fonts6.Add(1);
	lines6.Add("LEILA: What do we know about him?");
	fonts6.Add(1);
	lines6.Add("ABIGAIL: Well, when he learned that the two strongest heroes were on their way to his lair,");
	fonts6.Add(2);
	lines6.Add("ABIGAIL: he got scared that his evil plans would be ruined! ");
	fonts6.Add(2);
	lines6.Add("ABIGAIL: So, to protect himself he created powerful monsters to guard the forest and stop us from reaching him.");
	fonts6.Add(2);
	lines6.Add("LEILA: Like that will stop us!");
	fonts6.Add(1);
	lines6.Add("ABIGAIL: Of course it won't! But he believed that only the purest evil would be strong enough to stop us... ");
	fonts6.Add(2);
	lines6.Add("ABIGAIL: so he created each monster using a piece of his black heart. The monsters are all a part of him.");
	fonts6.Add(2);
	lines6.Add("LEILA: Hmm so that must mean if we defeat the monsters Zorrander becomes weaker?");
	fonts6.Add(1);
	lines6.Add("ABIGAIL: Yes, but each monster will try their best to keep us out of the forest.");
	fonts6.Add(2);
	lines6.Add("ABIGAIL: They can see our deepest darkest fears and will use that to try and scare us away.");
	fonts6.Add(2);
	lines6.Add("LEILA: ...They know our deepest darkest fears...?");
	fonts6.Add(1);
	lines6.Add("ABIGAIL: Yup but that's ok because we're the bravest heroes!");
	fonts6.Add(2);
	lines6.Add("LEILA: You're right, they won't scare us.");
	fonts6.Add(1);
	lines6.Add("ABIGAIL: Leila, what's your greatest fear?");
	fonts6.Add(2);
	lines6.Add("LEILA: ... My greatest fear is... losing you.");
	fonts6.Add(1);
	lines6.Add("ABIGAIL: Losing me? That seems pretty lame. Don't worry, we'll always be together! Right?");
	fonts6.Add(2);
	lines6.Add("LEILA: Of course silly... ");
	fonts6.Add(1);
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
			DialogueHandler->sendNewdialogueSequence(lines0, durations0, fonts0);
			dialogueSent = true;			
		}
		break;

	case 1:
		if (!dialogueSent) {
			DialogueHandler->sendNewdialogueSequence(lines1, durations1, fonts1);
			dialogueSent = true;			
		}
		break;

	case 2:
		if (!dialogueSent) {
			DialogueHandler->sendNewdialogueSequence(lines2, durations2, fonts2);
			dialogueSent = true;
			
		}
		break;

	case 3:
		if (!dialogueSent) {	
			DialogueHandler->sendNewdialogueSequence(lines3, durations3, fonts3);
			dialogueSent = true;
			
		}
		break;

	case 4:
		if (!dialogueSent) {
			DialogueHandler->sendNewdialogueSequence(lines4, durations4, fonts4);
			dialogueSent = true;
		}
		break;

	case 5:
		if (!dialogueSent) {
			DialogueHandler->sendNewdialogueSequence(lines5, durations5, fonts5);
			dialogueSent = true;
		}
		break;

	case 6:
		if (!dialogueSent) {
			DialogueHandler->sendNewdialogueSequence(lines6, durations6, fonts6);
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
			sendInputText(inputs0, inputFonts);

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
			sendInputText(inputs1, inputFonts);
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
			sendInputText(inputs2, inputFonts);

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
			sendInputText(inputs3, inputFonts);

			if (archer->GetInputAttack() || archer->GetAttacking()) ArcherDidInput = true;
			if (knight->GetBlocking()) KnightDidInput = true;

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
			sendInputText(inputs4, inputFonts);

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
void AJTutorial::sendInputText(TArray<FString> inputs, TArray<int> fonts) {
	if (!inputSent) {
		DialogueHandler->sendInputText(inputs, fonts);
		inputSent = true;
	}
}

//Readies JTutorial for sending text after an input prompt
void AJTutorial::postInput() {
	KnightDidInput = false;
	ArcherDidInput = false;
	inputSent = false;
}


