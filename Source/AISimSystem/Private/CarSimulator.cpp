// Fill out your copyright notice in the Description page of Project Settings.


#include "CarSimulator.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ACarSimulator::ACarSimulator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	CarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarMesh"));
	CarMesh->SetupAttachment(Root);

	Speed = FMath::RandRange(500, 600);
	CurrentSpeed = Speed;
}

// Called when the game starts or when spawned
void ACarSimulator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACarSimulator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
