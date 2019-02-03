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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TArray<FString> RecentlyHitBy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TArray<float> RecentlyHitByTimes;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void UpdateHitTimes(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float maxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float HealthPercent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float OldHealthPercent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float HitTrackingTime;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE float GetHealth() const { return Health; };

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void SetHealth(float NewHealth);

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void AddHealth(float Change);

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void AddHit(FString MoveName);

	UFUNCTION(BlueprintCallable, Category = "Combat")
		bool CheckHit(FString MoveName);
};
