// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "PlaygroundHeroesGameMode.h"
#include "PlaygroundHeroesCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "JHero.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "JKnight.h"

APlaygroundHeroesGameMode::APlaygroundHeroesGameMode()
{
	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/JArcherBP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/
}

void APlaygroundHeroesGameMode::BeginPlay()
{
	//UWorld* const World = GetWorld();
	//AJKnight* const archer = World->SpawnActor<AJKnight>();
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass2(TEXT("/Game/Blueprints/JKnightBP"));
	if (PlayerPawnBPClass2.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass2.Class;
	}*/
	APlayerController* PController = UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
	//PController->Possess(archer);
}
