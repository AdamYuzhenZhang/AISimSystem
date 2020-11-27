// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TextRenderComponent.h"
#include "Char_Agent.generated.h"

// Actions Enum
UENUM(BlueprintType)
enum class ETasks : uint8
{
	IDLING,
	SLEEPING,
	WORKING,
	EATING,
	SIGHTSEEING,
	SHOPPING,
	WORKINGOUTSIDE,
};

UCLASS()
class AISIMSYSTEM_API AChar_Agent : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AChar_Agent();

	UPROPERTY(EditAnywhere, Category = "Behavior")
		class UBehaviorTree* BotBehavior;

	// if stay here, then use 24 hour task list, else just go to destination and only visit attractor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StayHereOrNot")
		bool StayHere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
		ETasks CurrentTask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
		TArray<ETasks> TaskList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
		AActor* FinalTargetActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FText CharText;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
