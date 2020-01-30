// Copyright Alex Le 2019


#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

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

	FindPhysicsHandle();
	SetupInputComponent();


}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();																//Check for physics handle component
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle component on this object: %s"), *GetOwner()->GetName());
	}	
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();																		//Check for input (should never be missing but who knows)
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);																	// When the input is pressed then "this" Grabber component will run the function Grab (&UGrabber::Grab returns the address of this function)
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::ReleaseGrab);																	
	}	
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab has been pressed"));

	FVector LineTraceEnd = GetLineTraceEnd();

	// try and reach any actors with physics body collision channel set
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	// If we hit something, attach physics handle
	if (HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd
		);		
	}
	

}

void UGrabber::ReleaseGrab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab has been released"));

	// Remove physics handle upon release
	PhysicsHandle->ReleaseComponent();
}

FVector UGrabber::GetLineTraceEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(																					// Notice here that I put the arguments down on a new line, which looks like I interrupted the code,
		OUT PlayerViewPointLocation, 																											// But this is fine to do for the sake of readability without affecting the code.
		OUT PlayerViewPointRotation);																											// Macro at work; its here to tell us that those 2 variables will be edited by the function; 
																																				// useful since the function returns nothing obvious

	// UE_LOG(LogTemp, Warning, TEXT("GetPlayerViewPoint is at work: Location being %s and Rotation being %s"), 
	// *PlayerViewPointLocation.ToString(), 
	// *PlayerViewPointRotation.ToString());

	 FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;												// Our ray trace end distance, starting from PlayerViewPointLocation ane ending wherever the player would look at (rotation * reach)

	return LineTraceEnd;
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector LineTraceEnd = GetLineTraceEnd();

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
	

}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(																					// Notice here that I put the arguments down on a new line, which looks like I interrupted the code,
		OUT PlayerViewPointLocation, 																											// But this is fine to do for the sake of readability without affecting the code.
		OUT PlayerViewPointRotation);																											// Macro at work; its here to tell us that those 2 variables will be edited by the function; 
																																				// useful since the function returns nothing obvious

	// UE_LOG(LogTemp, Warning, TEXT("GetPlayerViewPoint is at work: Location being %s and Rotation being %s"), 
	// *PlayerViewPointLocation.ToString(), 
	// *PlayerViewPointRotation.ToString());

	 FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;												// Our ray trace end distance, starting from PlayerViewPointLocation ane ending wherever the player would look at (rotation * reach)

	// DrawDebugLine(																															// These are just for debugging purposes
	// 	GetWorld(),
	// 	PlayerViewPointLocation,
	// 	LineTraceEnd,
	// 	FColor(0,255,0),
	// 	false,
	// 	0.f,
	// 	0,
	// 	5.f
	// );

	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT(" ")), false, GetOwner());

	// Raycast out to a distance (Reach)																										// Trace a ray against the world using object type (4th argument) and return the first blocking Hit
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
		);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	 {
	 UE_LOG(LogTemp, Error, TEXT("This actor has been hit: %s"), *(ActorHit)->GetName());
	 }



	return Hit;
}

