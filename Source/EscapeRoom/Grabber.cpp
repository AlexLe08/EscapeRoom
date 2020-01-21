// Copyright Alex Le 2019


#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Grabber.h"

#define OUT																																		// This lets us define a macro, which in terms of code does not do anything except mark up parameters for our readability

// Sets default values for this component's properties
UGrabber::UGrabber()
{

	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("Got the grabber"));
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(																					// Notice here that I put the arguments down on a new line, which looks like I interrupted the code,
		OUT PlayerViewPointLocation, 																											// But this is fine to do for the sake of readability without affecting the code.
		OUT PlayerViewPointRotation);																											// Macro at work; its here to tell us that those 2 variables will be edited by the function; 
																																				// useful since the function returns nothing obvious

	// UE_LOG(LogTemp, Warning, TEXT("GetPlayerViewPoint is at work: Location being %s and Rotation being %s"), 
	// *PlayerViewPointLocation.ToString(), 
	// *PlayerViewPointRotation.ToString());

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(0,255,0),
		false,
		0.f,
		0,
		5.f
	);

	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT(" ")), false, GetOwner());

	// Raycast out to a distance (Reach)
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
		);

	AActor* ActorHit = Hit.GetActor();
	if(ActorHit)
	{
	UE_LOG(LogTemp, Error, TEXT("This actor has been hit: %s"), *(ActorHit)->GetName());
	}



	// See what it hits
}

