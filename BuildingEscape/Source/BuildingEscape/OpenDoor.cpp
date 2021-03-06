// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *GetOwner()->GetName());
	}

	// Dont need this anymore so commenting it out
	// ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}

/*
void UOpenDoor::OpenDoor()
{

	// Create a rotator (this thing is in pitch, yaw, and roll, i think in that order. so i think that means:
	// X=roll, Y=pitch, Z=yaw)
	// FRotator NewRotation = FRotator(0.0f, -60.0f, 0.0f);
	// ^ we dont need this anymore since we stuck this directly in the SetActorRotation method...

	// Set the door rotation
	// Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
	OnOpen.Broadcast();
}
*/

/*
void UOpenDoor::CloseDoor()
{
	// Set the door rotation
	if (!Owner) { 
		UE_LOG(LogTemp, Error, TEXT("Owner not found!"));
		return;
	}
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}
*/

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	// If the ActorThatOpens is in the volume
	// if (PressurePlate->IsOverlappingActor(ActorThatOpens)) - we don't need this anyjmore
	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		//OpenDoor();
		OnOpen.Broadcast();
		//LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	//if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	else
	{
		//CloseDoor();
		OnClose.Broadcast();
	}
	// GetWorld()->GetTimeSeconds();
	// Check if its time to close the door
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	// iterate trhough them adding their masses
	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName());
	}

	return TotalMass;
}