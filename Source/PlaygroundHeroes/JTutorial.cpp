// Fill out your copyright notice in the Description page of Project Settings.

#include "JTutorial.h"
#include <EngineGlobals.h>
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
			archer = ArcherItr;
		}
	}
	
	
}

void AJTutorial::DisplayText() {
	switch (Step)
	{
	case 0:
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
		if (canAdvance) {
			Step++;
		}
		break;
	case 1: //Any Movement
		//DisplayText();
		if (canAdvance) {
			if (archer->GetInputDirection().X != 0 || archer->GetInputDirection().Y != 0
				|| knight->GetInputDirection().X != 0 || knight->GetInputDirection().Y != 0) {
				Step++;
			}
		}
		break;
	case 2: //knight right stick and RB
		//DisplayText();
		if (canAdvance) {
			if (/*knight->GetLockTarget() &&*/ (knight->GetInputAttack() || knight->GetAttacking()) 
				&& (archer->GetInputDodge() || archer->GetDodging() ) ) {
				Step++;
			}
		}
		break;
	case 3:
		//DisplayText
		if (canAdvance) {
			if (/*archer->GetLockTarget() &&*/ (archer->GetInputAttack() || archer->GetAttacking())
				&& (knight->GetInputDodge() || knight->GetDodging() )) {
				Step++;
			}
		}
		break;
	case 4:
		//DisplayText();
		if (canAdvance) {
			if (/*archer->GetLockTarget() &&*/ (archer->GetInputAttack() || archer->GetAttacking())
				&& (knight->GetInputDodge() || knight->GetDodging() )) {
				Step++;
			}
		}
		break;
	default:
		break;
	}
}
