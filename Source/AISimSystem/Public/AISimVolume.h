// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AISimVolume.generated.h"

UCLASS()
class AISIMSYSTEM_API AAISimVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAISimVolume();

	UPROPERTY(BlueprintReadOnly)
		USceneComponent* Root;
	UPROPERTY(BlueprintReadOnly)
		USceneComponent* CameraRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* SphereVolume;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "ClippingViewerAngle")
		void SetView(FRotator Rotation);
	// Pitch Y
	UFUNCTION(BlueprintCallable, Category = "ClippingViewerAngle")
		void AddPitch(float DeltaPitch);
	// Yaw Z
	UFUNCTION(BlueprintCallable, Category = "ClippingViewerAngle")
		void AddYaw(float DeltaYaw);
	UFUNCTION(BlueprintCallable, Category = "ClippingViewerAngle")
		FRotator GetCurrentRotator();
};
