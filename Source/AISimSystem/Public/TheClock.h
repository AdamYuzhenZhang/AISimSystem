// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "TheClock.generated.h"

UCLASS()
class AISIMSYSTEM_API ATheClock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATheClock();

	UFUNCTION(BlueprintCallable)
		void SpeedUp();

	UFUNCTION(BlueprintCallable)
		void SpeedDown();

	UFUNCTION(BlueprintCallable)
		void SetSpeed(float speed);

	UFUNCTION(BlueprintCallable)
		void AddHour();

	UFUNCTION(BlueprintCallable)
		void MinusHour();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Time")
		int SpeedScale;
	UPROPERTY(EditAnywhere, Category = "Time")
		int StartHour;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Time")
		int Hour;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Time")
		int Minute;

	UPROPERTY(BlueprintReadWrite, Category = "Time")
		FText TimeTextString;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* Root;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//	UTextRenderComponent* TimeText;

	UPROPERTY(EditAnywhere, Category = "Sun")
		AActor* TheBPSun;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int Second = 0;
	int Day = 0;
	int SpeedScaleInt;

	// repeate every second
	void RepeatingFunction(float DeltaTime);
	void SetPrintingText();
	void UpdateSunPosition();

	FTimerHandle MemberTimerHandle;
};
