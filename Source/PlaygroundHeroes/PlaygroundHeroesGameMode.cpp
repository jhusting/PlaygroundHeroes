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
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/JHero"));
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
	
	//SpawnBothPlayers(World);
}

/*
void APlaygroundHeroesGameMode::SpawnBothPlayers(UWorld* World) {
	//create 2nd player with controller id 1
	UGameplayStatics::CreatePlayer(World, 1, true);

	//create knight up in the air
	AJHero* knight = World->SpawnActor<AJHero>(  FVector(0, 0, 2000));
	APlayerController::Possess(knight, UGameplayStatics::GetPlayerController(GetWorld(), 0));

	//create archer up in the air
	AJHero* archer = World->SpawnActor<AJHero>(FVector(0, 0, 2000));
	APlayerController::Possess(archer, UGameplayStatics::GetPlayerController(World, 1));
}
*/