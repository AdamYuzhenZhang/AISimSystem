// Fill out your copyright notice in the Description page of Project Settings.


#include "Char_Agent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/NameTypes.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AChar_Agent::AChar_Agent()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Turn character to direction moving
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = FMath::RandRange(150, 200);

	//CharText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("CharacterText"));
	//CharText->SetWorldSize(20.0f);
	//CharText->SetTextRenderColor(FColor::Red);
	//CharText->SetHorizontalAlignment(EHTA_Center);

	TaskList.Init(ETasks::IDLING, 24);
	StayHere = true;
}

// Called when the game starts or when spawned
void AChar_Agent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChar_Agent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChar_Agent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

