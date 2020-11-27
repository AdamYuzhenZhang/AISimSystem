// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Agent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"
#include "Navigation/CrowdFollowingComponent.h"

AAI_Agent::AAI_Agent(const FObjectInitializer& ObjectInitializer)
	//:Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));

	PrimaryActorTick.bCanEverTick = true;
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	/*SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;*/

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	void OnPawnDetected(const TArray<AActor*> & DetectedPawns);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);

	SightseeingIsSet = false;
}

void AAI_Agent::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Char = Cast<AChar_Agent>(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("On Possess"));

	if (Char && Char->BotBehavior) {
		BlackboardComp->InitializeBlackboard(*Char->BotBehavior->BlackboardAsset);
		UE_LOG(LogTemp, Warning, TEXT("Initialize Blackboard"));
		// Get value from Blackboard
		/*TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("Target"));*/
		BehaviorComp->StartTree(*Char->BotBehavior);

		TaskList = Char->TaskList;
		FinalTargetActor = Char->FinalTargetActor;
	}
}

void AAI_Agent::BeginPlay()
{
	Super::BeginPlay();
}

void AAI_Agent::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (BlackboardComp->GetValueAsBool(FName("ShouldBeDestroyed"))) {
		Destroy();
	}

	if (Char->StayHere) {
		UseTaskListForBehavior();
	}
	else {
		DoNotUseTaskList();
	}
}

FRotator AAI_Agent::GetControlRotation() const
{
	if (GetPawn() == nullptr) {
		return FRotator(0.0f, 0.0f, 0.0f);
	}
	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0F);
}

void AAI_Agent::SetNewTarget()
{
	TArray<AActor*> ReturnedPlaces;
	FOutputDeviceNull ar;

	// Assign target and CharText based on CurrentTask
	switch (CurrentTask) {
	case ETasks::EATING:
		// Set target and update the printing text of character
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("RESTAURANT"), ReturnedPlaces);
		Char->CharText = FText::FromString("EATING");
		break;
	case ETasks::WORKING:
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("OFFICE"), ReturnedPlaces);
		Char->CharText = FText::FromString("WORKING");
		break;
	case ETasks::WORKINGOUTSIDE:
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("GATE"), ReturnedPlaces);
		Char->CharText = FText::FromString("WORKINGOUTSIDE");
		break;
	case ETasks::SHOPPING:
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SHOP"), ReturnedPlaces);
		Char->CharText = FText::FromString("SHOPPING");
		break;

		// case idling and sightseeing are dealed in behavior tree
	case ETasks::IDLING:
		Char->CharText = FText::FromString("IDLING");
		SightseeingIsSet = false;
		break;

		// case sleeping: go back to the origin where actor was spawned
	case ETasks::SLEEPING:
		BlackboardComp->SetValueAsObject(FName("Target"), Char->FinalTargetActor);
		Char->CharText = FText::FromString("SLEEPING");
		break;
	}

	if (ReturnedPlaces.Num() > 0) {
		bool placeNotChosen = true;
		UE_LOG(LogTemp, Warning, TEXT("Chosing Place to go to"));
		while (placeNotChosen) {
			// Select random location and set as new target
			int RandIndex = FMath::RandRange(0, ReturnedPlaces.Num() - 1);

			ALocation* randomLocation = Cast<ALocation>(ReturnedPlaces[RandIndex]);

			if (randomLocation->CurrentNumber < randomLocation->Capacity) {
				randomLocation->CurrentNumber += 1;
				/*AActor* RandomTarget = ReturnedPlaces[RandIndex];*/
				BlackboardComp->SetValueAsObject(FName("Target"), randomLocation);
				placeNotChosen = false;
			}
		}
	}
}

void AAI_Agent::Sightseeing()
{
	BlackboardComp->SetValueAsEnum(FName("CurrentTask"), static_cast<uint8>(CurrentTask));
	Char->CharText = FText::FromString("SIGHTSEEING");
	VisitedAttractor.Add(SightseeingActor);
	BlackboardComp->SetValueAsObject(FName("Target"), SightseeingActor);

	// if timer is not active, start timer, end sightseeing at the end of timer
	if (!GetWorld()->GetTimerManager().IsTimerActive(SightseeingDuration)) {

		float sightseeingTime = FMath::RandRange(SightseeingActor->AttractionDurationMin, SightseeingActor->AttractionDurationMax);

		GetWorld()->GetTimerManager().SetTimer(SightseeingDuration, this, &AAI_Agent::EndSightseeing, sightseeingTime, false);
	}
	SightseeingIsSet = true;
}

void AAI_Agent::EndSightseeing()
{
	CurrentTask = ETasks::IDLING;
	BlackboardComp->SetValueAsEnum(FName("CurrentTask"), static_cast<uint8>(CurrentTask));
	Char->CurrentTask = CurrentTask;
	Char->CharText = FText::FromString("IDLING");
	if (!GetWorld()->GetTimerManager().IsTimerActive(SightseeingReset)) {
		// rest 10 seconds between sightseeings
		GetWorld()->GetTimerManager().SetTimer(SightseeingReset, this, &AAI_Agent::ResetSightseeing, 10.f, false);
	}
}

void AAI_Agent::ResetSightseeing()
{
	SightseeingIsSet = false;
}

void AAI_Agent::UseTaskListForBehavior()
{
	ETasks NewTask = TaskList[CurrentHour];
	//UE_LOG(LogTemp, Warning, TEXT("Tasklist for Behavior"));
	// sightseeing
	if (CurrentTask == ETasks::SIGHTSEEING && NewTask == ETasks::IDLING) {
		if (SightseeingIsSet == false) {
			Sightseeing();
		}
	}
	else if (CurrentTask != NewTask) {
		CurrentTask = NewTask;
		/*BlackboardComp->SetValueAsBool(FName("NewTask"), true);*/
		BlackboardComp->SetValueAsEnum(FName("CurrentTask"), static_cast<uint8>(CurrentTask));

		Char->CurrentTask = CurrentTask;

		SetNewTarget();
	}
}

void AAI_Agent::DoNotUseTaskList()
{
	BlackboardComp->SetValueAsObject(FName("Target"), Char->FinalTargetActor);
	BlackboardComp->SetValueAsBool(FName("PedMovingToTarget"), true);
	BlackboardComp->SetValueAsEnum(FName("CurrentTask"), static_cast<uint8>(ETasks::IDLING));
	Char->CharText = FText::FromString("IDLING");

	if (CurrentTask == ETasks::SIGHTSEEING) {
		if (SightseeingIsSet == false) {
			BlackboardComp->SetValueAsBool(FName("PedMovingToTarget"), false);
			Sightseeing();
		}
	}
	else {
		BlackboardComp->SetValueAsBool(FName("PedMovingToTarget"), true);
		BlackboardComp->SetValueAsObject(FName("Target"), Char->FinalTargetActor);
	}
}
