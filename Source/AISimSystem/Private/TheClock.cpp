// Fill out your copyright notice in the Description page of Project Settings.


#include "TheClock.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"

// Sets default values
ATheClock::ATheClock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpeedScale = 1;

	Day = 0;
	Hour = 0;
	Minute = 0;
	Second = 0;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root);

	/*TimeText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TimeText"));
	TimeText->SetWorldSize(70.0f);
	TimeText->SetTextRenderColor(FColor::Red);
	TimeText->SetHorizontalAlignment(EHTA_Center);
	TimeText->SetupAttachment(GetRootComponent());*/
}

// Called when the game starts or when spawned
void ATheClock::BeginPlay()
{
	Super::BeginPlay();
	
	/*GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &ATheClock::RepeatingFunction, 1.0f, true);*/
	Hour += StartHour;
}

// Called every frame
void ATheClock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RepeatingFunction(DeltaTime);
}

void ATheClock::SpeedUp()
{
	if (SpeedScale < 1000) {
		SpeedScale = SpeedScale * 10;
	}
}

void ATheClock::SpeedDown()
{
	if (SpeedScale > 1) {
		SpeedScale = SpeedScale / 10;
	}
}

void ATheClock::SetSpeed(float speed)
{
	SpeedScale = speed;
}

void ATheClock::AddHour()
{
	Hour += 1;
	if (Hour == 24) {
		Day += 1;
		Hour = 0;
	}
}

void ATheClock::MinusHour()
{
	Hour -= 1;
	if (Hour < 0) {
		Day -= 1;
		Hour = 23;
	}
}

// Count The Time
void ATheClock::RepeatingFunction(float DeltaTime)
{
	Second += SpeedScale * DeltaTime;
	if (Second >= 60) {
		int min = Second / 60;
		int s = Second % 60;
		Minute += min;
		Second = s;
		if (Minute >= 60) {
			int h = Minute / 60;
			int m = Minute % 60;
			Hour += h;
			Minute = s;
			if (Hour >= 24) {
				int d = Hour / 24;
				int h2 = Hour % 24;
				Day += d;
				Hour = h2;
			}
		}
	}

	SetPrintingText();
	UpdateSunPosition();
}

void ATheClock::SetPrintingText()
{
	FString TimeString = FString::FromInt(Day) + " Day " + FString::FromInt(Hour) + ":" + FString::FromInt(Minute);
	TimeTextString = FText::FromString(TimeString);
	//TimeText->SetText(TimeTextString);
}

void ATheClock::UpdateSunPosition()
{
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("UpdateSunPos"), ar, NULL, true);
}

