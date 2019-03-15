// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JEnemy.generated.h"

UCLASS()
class PLAYGROUNDHEROES_API AJEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AJEnemy();

	/*
		These two lists work in tandem to aid in hit tracking
		RecentlyHitBy will keep track of the names of the moves it's been recently hit by
		For instance: "KnightAttack1, ArcherAttack1"
		RecentlyHitByTimes will store the time since that hit was first registered in seconds
		For instance 0.66f, 0.3f

		RecentlyHitBy[0] and RecentlyHitByTimes[0] should be paired together
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TArray<FString> RecentlyHitBy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TArray<float> RecentlyHitByTimes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Classes")
		TSubclassOf<class UUserWidget> DamageWidgetBPClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
		Adds DeltaTime to all the elemenys in RecentlyHitByTimes

		Also deletes hits that have been tracked for more than HitTrackingTime seconds
	*/
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
	// The time, in seconds, we will keep track of hits
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float HitTrackingTime;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE float GetHealth() const { return Health; };

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void SetHealth(float NewHealth);

	UFUNCTION(BlueprintCallable, Category = "Combat")
		virtual void AddHealth(float Change, FString MoveName);

	// Adds MoveName to the recently hit by list
	// Knight Move Names: KnightAttack1, KnightAttack2, KnightAttack3
	// Archer Move Names: ArcherHit1
	UFUNCTION(BlueprintCallable, Category = "Combat")
		void AddHit(FString MoveName);

	// Checks if MoveName is in the hit list
	// Knight Move Names: KnightAttack1, KnightAttack2, KnightAttack3
	// Archer Move Names: ArcherHit
	UFUNCTION(BlueprintCallable, Category = "Combat")
		bool CheckHit(FString MoveName);
};
