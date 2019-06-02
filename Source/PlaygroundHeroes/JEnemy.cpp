// Fill out your copyright notice in the Description page of Project Settings.

#include "JEnemy.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Engine/World.h"

// Sets default values
AJEnemy::AJEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 100.f;
	HitTrackingTime = .75f;
}

// Called when the game starts or when spawned
void AJEnemy::BeginPlay()
{
	Super::BeginPlay();
	maxHealth = Health;
	HealthPercent = 1.0f;

}

void AJEnemy::UpdateHitTimes(float DeltaTime)
{
	for (int32 i = 0; i < RecentlyHitByTimes.Num(); ++i)
	{
		RecentlyHitByTimes[i] += DeltaTime;

		if (RecentlyHitByTimes[i] >= HitTrackingTime)
		{
			RecentlyHitBy.RemoveAt(i);
			RecentlyHitByTimes.RemoveAt(i);
		}
	}
}

// Called every frame
void AJEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HealthPercent = Health / maxHealth;
	UpdateHitTimes(DeltaTime);
}

void AJEnemy::SetHealth(float NewHealth)
{
	Health = NewHealth;
}

void AJEnemy::AddHealth(float Change, FString MoveName)
{
	FActorSpawnParameters SpawnParams;

	UWorld* const World = GetWorld();
	FConstPlayerControllerIterator pItr = World->GetPlayerControllerIterator();

	//Only spawn damage text widget on something that isn't already dead
	if (this->Health > 0) {
		// Spawn Player 1 Damage Text
		UUserWidget* DamageText = CreateWidget<UUserWidget>(World, DamageWidgetBPClass);
		DamageText->SetOwningPlayer(Cast<APlayerController>(*pItr));

		if (DamageText)
		{
			UProperty* Property = DamageText->GetClass()->FindPropertyByName("DamageToDisplay");
			if (Property) // If we successfully found that property
			{
				float* currDamage = Property->ContainerPtrToValuePtr<float>(DamageText);
				if (currDamage) //If the value has been initialized
					*currDamage = -1.f * Change; // Damage = 15 + 20 * the held ratio (this would be 100% at max strength, 0% with a 1 frame hold)
			}


			Property = DamageText->GetClass()->FindPropertyByName("HitActor");
			if (Property)
			{
				AActor** hitActor = Property->ContainerPtrToValuePtr<AActor*>(DamageText);
				*hitActor = this;
			}

			DamageText->AddToPlayerScreen();
		}

		pItr++;

		DamageText = CreateWidget<UUserWidget>(World, DamageWidgetBPClass);
		DamageText->SetOwningPlayer(Cast<APlayerController>(*pItr));

		if (DamageText)
		{
			UProperty* Property = DamageText->GetClass()->FindPropertyByName("DamageToDisplay");
			if (Property) // If we successfully found that property
			{
				float* currDamage = Property->ContainerPtrToValuePtr<float>(DamageText);
				if (currDamage) //If the value has been initialized
					*currDamage = -1.f * Change; // Damage = 15 + 20 * the held ratio (this would be 100% at max strength, 0% with a 1 frame hold)
			}

			Property = DamageText->GetClass()->FindPropertyByName("HitActor");
			if (Property)
			{
				AActor** hitActor = Property->ContainerPtrToValuePtr<AActor*>(DamageText);
				*hitActor = this;
			}

			DamageText->AddToPlayerScreen();
		}
	}

	Health = FMath::Clamp(Health + Change, 0.f, maxHealth);
	AddHit(MoveName);
}

void AJEnemy::AddHit(FString MoveName)
{
	if (!RecentlyHitBy.Contains(MoveName))
	{
		RecentlyHitBy.Add(MoveName);
		RecentlyHitByTimes.Add(0.f);
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("Warning: Trying to add a hit that is already an element of RecentlyHitBy"));
	}
}

bool AJEnemy::CheckHit(FString MoveName)
{
	return RecentlyHitBy.Contains(MoveName);
}