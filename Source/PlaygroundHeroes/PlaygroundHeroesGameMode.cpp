// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "PlaygroundHeroesGameMode.h"
#include "PlaygroundHeroesCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "JHero.h"
#include "Runtime/Engine/Classes/Engine/World.h"

APlaygroundHeroesGameMode::APlaygroundHeroesGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/JKnightBP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void APlaygroundHeroesGameMode::BeginPlay()
{
	UWorld* const World = GetWorld();
	AJHero* const archer = World->SpawnActor<AJHero>();
	UGameplayStatics::CreatePlayer(archer);
}
