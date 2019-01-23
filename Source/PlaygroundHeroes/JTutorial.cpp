// Fill out your copyright notice in the Description page of Project Settings.

#include "JTutorial.h"

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

void AJTutorial::KnightLockOn() {

}

void AJTutorial::KnightAttackPressed() {

}

// Called every frame
void AJTutorial::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	switch (Step) {
	case 0:
		//DisplayText[text1, text2, text3] //intro text about world
		//Wait for X seconds: break
		break;
	case 1: //Any Movement 
	//	if (archer.GetInputAxisValue("Move Forward") != 0 || archer.GetInputAxisValue("Move Right") 
	//		|| knight.GetInputAxisValue("Move Forward") || knight.GetInputAxisValue("Move Right") )
	{
		Step++;
	}
	break;
	case 2: //knight right stick and RB
//		knight.InputComponent->BindAction()
//		knight.InputComponent->BindAction()


	default:
		break;
	}
}

