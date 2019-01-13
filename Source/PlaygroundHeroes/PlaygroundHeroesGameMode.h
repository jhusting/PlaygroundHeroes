// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlaygroundHeroesGameMode.generated.h"

UCLASS(minimalapi)
class APlaygroundHeroesGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APlaygroundHeroesGameMode();

	virtual void BeginPlay() override;

	void SpawnBothPlayers(UWorld* world);
};



