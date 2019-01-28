// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JEnemy.generated.h"

UCLASS()
class PLAYGROUNDHEROES_API AJEnemy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AJEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float maxHealth;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE float GetHealth() const { return Health; };

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void SetHealth(float NewHealth);

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void AddHealth(float Change);
};
