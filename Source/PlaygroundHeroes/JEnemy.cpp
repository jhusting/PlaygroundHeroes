// Fill out your copyright notice in the Description page of Project Settings.

#include "JEnemy.h"


// Sets default values
AJEnemy::AJEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 100.f;
	maxHealth = Health;
}

// Called when the game starts or when spawned
void AJEnemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AJEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJEnemy::SetHealth(float NewHealth)
{
	Health = NewHealth;
}

void AJEnemy::AddHealth(float Change)
{
	Health = FMath::Clamp(Health + Change, 0.f, 100.f);
}