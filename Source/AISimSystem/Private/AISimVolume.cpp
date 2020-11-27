// Fill out your copyright notice in the Description page of Project Settings.


#include "AISimVolume.h"

// Sets default values
AAISimVolume::AAISimVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	CameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	CameraRoot->SetupAttachment(Root);
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(CameraRoot);

	SphereVolume = CreateDefaultSubobject<USphereComponent>(TEXT("BoundingSphere"));
	SphereVolume->SetupAttachment(GetRootComponent());

	CameraRoot->SetRelativeRotation(FRotator(-45.0, 45.0, 0.0));
}

// Called when the game starts or when spawned
void AAISimVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAISimVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAISimVolume::SetView(FRotator Rotation)
{
	CameraRoot->SetRelativeRotation(Rotation);
}

void AAISimVolume::AddPitch(float DeltaPitch)
{
	CameraRoot->AddRelativeRotation(FRotator(0.0, DeltaPitch, 0.0));
}

void AAISimVolume::AddYaw(float DeltaYaw)
{
	CameraRoot->AddRelativeRotation(FRotator(0.0, 0.0, DeltaYaw));
}

FRotator AAISimVolume::GetCurrentRotator()
{
	return CameraRoot->GetRelativeRotation();
}

