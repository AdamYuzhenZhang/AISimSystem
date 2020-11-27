// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"
#include "Char_Agent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Location.generated.h"

UCLASS()
class AISIMSYSTEM_API ALocation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALocation();

	UPROPERTY(BlueprintReadWrite)
		USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UTextRenderComponent* LocationText;

	/*Character Staying Here*/
	// Type of Character Staying Here
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStayingHere")
		TArray<TSubclassOf<class AChar_Agent>> CharacterToSpawn;
	// Number of Character to Spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStayingHere")
		int CharacterCount;
	// Location to Spawn
	UPROPERTY(BlueprintReadWrite)
		FVector SpawnLocation;

	/*Capacity of the Location*/
	// Number of Character the Location Can Hold
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int Capacity;
	// Number of Character Currently Here
	UPROPERTY(BlueprintReadWrite, Category = "Character")
		int CurrentNumber;
	// Whether to hide Chars at location
	UPROPERTY(BlueprintReadWrite, Category = "Character")
		bool HideActor;

	/*Values for Attractor*/
	// Int between 1-10; the possibility of character comming here; Only used for attractor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForAttractor")
		int AttractionLevel;
	// Sightseeing time is a random number between min and max
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForAttractor")
		int AttractionDurationMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForAttractor")
		int AttractionDurationMin;

	/*Values for Continuous Spawning*/
	// Should Spawn Character Continuously
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterPassingBy")
		bool ContinuousSpawing;
	// Spawning rate is between Min and Max
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterPassingBy")
		int DensityPerTenSecMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterPassingBy")
		int DensityPerTenSecMax;
	// Type of Character to Spawn for Countinuous Spawning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterPassingBy")
		TSubclassOf<class AChar_Agent> PedestrianToSpawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float tickCounter;

	void ContinuousSpawnCharacter();

	AActor* FindRandomExitGate();

	void SpawnCharacter();

	TSubclassOf<class AChar_Agent> GetRandomCharacterFromArray();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnPlayerEnter(UPrimitiveComponent* OverlapComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComponent,
			int32 OtherBodyIndex,
			bool bFramSweep,
			const FHitResult& SweepResult);
};
