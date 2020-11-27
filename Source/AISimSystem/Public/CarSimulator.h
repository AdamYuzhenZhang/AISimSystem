// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "CarSimulator.generated.h"

UCLASS()
class AISIMSYSTEM_API ACarSimulator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACarSimulator();

	UPROPERTY(BlueprintReadOnly)
		USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* CarMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USplineComponent* RoadSpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed;
	UPROPERTY(BlueprintReadWrite)
		float CurrentSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
