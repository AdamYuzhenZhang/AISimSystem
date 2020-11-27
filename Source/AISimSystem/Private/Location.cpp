// Fill out your copyright notice in the Description page of Project Settings.


#include "Location.h"
#include "Engine/EngineTypes.h"
#include "Misc/OutputDeviceNull.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALocation::ALocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	BoxComponent->SetupAttachment(GetRootComponent());
	/*BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALocation::OnPlayerEnter);*/

	LocationText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("LocationText"));
	LocationText->SetupAttachment(GetRootComponent());

	Capacity = INT16_MAX;

	AttractionLevel = 10;
	AttractionDurationMax = 120;
	AttractionDurationMin = 30;

	tickCounter = 0;

	HideActor = false;
}

// Called when the game starts or when spawned
void ALocation::BeginPlay()
{
	Super::BeginPlay();
	
	if (CharacterCount > 0) {
		SpawnCharacter();
	}
}

// Called every frame
void ALocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ContinuousSpawing) {
		tickCounter += DeltaTime;
		if (tickCounter > 2) {
			ContinuousSpawnCharacter();
			tickCounter = 0;
		}
	}
}

void ALocation::ContinuousSpawnCharacter()
{
	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	// Adjust to avoid colliding
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FRotator Rotator = FRotator(0, 0, 0);

	int RandomDensity = FMath::RandRange(DensityPerTenSecMin / 5, DensityPerTenSecMax / 5);

	FOutputDeviceNull ar;
	for (int i = 0; i < RandomDensity; i++) {
		this->CallFunctionByNameWithArguments(TEXT("RandomSpawnLocation"), ar, NULL, true);
		AChar_Agent* CharJustSpawned = World->SpawnActor<AChar_Agent>(PedestrianToSpawn, SpawnLocation, Rotator, SpawnParams);
		/*UE_LOG(LogTemp, Warning, TEXT("Spawning"));*/
		if (CharJustSpawned) {
			CharJustSpawned->FinalTargetActor = FindRandomExitGate();
		}
	}
}

// find a random exit gate, cannot be this location
AActor* ALocation::FindRandomExitGate()
{
	TArray<AActor*> ReturnedPlaces;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("GATE"), ReturnedPlaces);

	// remove this from returned places
	ReturnedPlaces.Remove(this);

	int RandIndex = FMath::RandRange(0, ReturnedPlaces.Num() - 1);
	return ReturnedPlaces[RandIndex];
}

void ALocation::SpawnCharacter()
{
	if (CharacterToSpawn.Num() != 0) {
		UWorld* World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			// Adjust to avoid colliding
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			FRotator Rotator = FRotator(0, 0, 0);

			// spawn actor at random location within the location actor
			for (int i = 0; i < CharacterCount; i++) {
				FOutputDeviceNull ar;
				this->CallFunctionByNameWithArguments(TEXT("RandomSpawnLocation"), ar, NULL, true);
				// select a random character first
				TSubclassOf<class AChar_Agent> selectedChar = GetRandomCharacterFromArray();
				AChar_Agent* CharJustSpawned = World->SpawnActor<AChar_Agent>(selectedChar, SpawnLocation, Rotator, SpawnParams);
				/*UE_LOG(LogTemp, Warning, TEXT("Spawning"));*/
				if (CharJustSpawned) {
					CharJustSpawned->FinalTargetActor = this;
				}
			}
		}
	}
}

TSubclassOf<class AChar_Agent> ALocation::GetRandomCharacterFromArray()
{
	int randIndex = FMath::RandRange(0, CharacterToSpawn.Num() - 1);
	return CharacterToSpawn[randIndex];
}

void ALocation::OnPlayerEnter(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFramSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		AChar_Agent* CollidedChar = Cast<AChar_Agent>(OtherActor);
		UE_LOG(LogTemp, Warning, TEXT("Collided with something"));
		if (CollidedChar) {
			UE_LOG(LogTemp, Warning, TEXT("its an character"));
			if (!CollidedChar->StayHere && CollidedChar->FinalTargetActor == this) {
				UE_LOG(LogTemp, Warning, TEXT("it should be destroyed"));
				CollidedChar->Destroy();
			}
		}
	}
}