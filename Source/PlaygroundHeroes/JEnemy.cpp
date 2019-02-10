// Fill out your copyright notice in the Description page of Project Settings.

#include "JEnemy.h"


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
<<<<<<< HEAD
{
	Health = FMath::Clamp(Health + Change, 0.f, 100.f);
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
=======
{
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
>>>>>>> 80d4c3eb4d83e85b5e54f76453055659a69adbf1
	return RecentlyHitBy.Contains(MoveName);
}