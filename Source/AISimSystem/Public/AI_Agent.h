// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Char_Agent.h"
#include "Location.h"
#include "AI_Agent.generated.h"

/**
 * 
 */
UCLASS()
class AISIMSYSTEM_API AAI_Agent : public AAIController
{
	GENERATED_BODY()
	
		UPROPERTY(transient)
		class UBehaviorTreeComponent* BehaviorComp;

public:

	AAI_Agent(const FObjectInitializer& ObjectInitializer);

	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation() const override;

	/*UFUNCTION()
	void OnPawnDetected(TArray<AActor*> DetectedPawns);*/

	UPROPERTY(visibleAnywhere, BlueprintReadOnly)
		class UBlackboardComponent* BlackboardComp;

	/* AI Perception Component*/
	UPROPERTY(visibleAnywhere, BlueprintReadWrite, Category = AI)
		UAIPerceptionComponent* AIPerceptionComponent;
	// AI Sight Radius
	UPROPERTY(visibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightRadius = 500.0f;
	// AI Sight Age
	UPROPERTY(visibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightAge = 5.0f;
	// AI Lose Sight Radius
	UPROPERTY(visibleAnywhere, BlueprintReadWrite, Category = AI)
		float AILoseSightRadius = AISightRadius + 50.0f;
	// AI Field of View
	UPROPERTY(visibleAnywhere, BlueprintReadWrite, Category = AI)
		float AIFieldOfView = 90.0f;
	// AI Sight Configuration
	UPROPERTY(visibleAnywhere, BlueprintReadWrite, Category = AI)
		class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
		ETasks CurrentTask;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Task")
		TArray<ETasks> TaskList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
		AActor* FinalTargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
		int32 CurrentHour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		AChar_Agent* Char;

	AActor* TargetActor;

	// stores the visited attractors
	UPROPERTY(BlueprintReadWrite)
		TArray<ALocation*> VisitedAttractor;

	UPROPERTY(BlueprintReadWrite)
		ALocation* SightseeingActor;

	// Timer dealing with the duration of sightseeing
	FTimerHandle SightseeingDuration;

	// Timer dealing with the resting time between sightseeings
	FTimerHandle SightseeingReset;

protected:
	void SetNewTarget();

	void Sightseeing();

	void EndSightseeing();

	void ResetSightseeing();

	bool SightseeingIsSet;

	void UseTaskListForBehavior();

	void DoNotUseTaskList();
};
