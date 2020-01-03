// Copyright Alex Le 2019


#include "OpenDoor.h"
#include "GameFramework/Actor.h"

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

	InitialYaw = GetOwner()->GetActorRotation().Yaw;														//Starting yaw in the world; it could be 0 it could be 270 etc
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;																			//Door turns by adding to the initial yaw over time, eventually rotating from its oroginal yaw by x degrees, which is set as an editable parameter in the editor

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);											// Delta time is the time between frames being rendered

	UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);

	// Door rotates but it rotates from an absolute position instead of relative
	// float StartingYaw = GetOwner()->GetActorRotation().Yaw;												// Gets the actor's rotation, wherever it is in the world; it could be 0 iot could be 270 who knows
	// FRotator OpenDoor = FRotator(0.0f,TargetYaw,0.0f);

	// Lerp(StartingValue, TargetValue, value bnetween 0 - 1.0f)											// Lerp is linear interpolation, or how fast a number changes from its start to target

	// OpenDoor.Yaw = FMath::Lerp(StartingYaw, TargetYaw, 0.1f);											// Every frame the rotator gets a new starting yaw,
	// GetOwner()->SetActorRotation(OpenDoor);																// Which then gets set as the actor's new rotation

	
	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, 0.01f * DeltaTime);										// Current yaw is set each frame to a new value that eventually reaches the target yaw, 
	//																										// ALSO we multiple by Delta time to make the change over time independent from frame rates so the velocity is constant on slow and fast PCS

	FRotator DoorRotation = GetOwner()->GetActorRotation();													// A rotator is created and assigned to be the actor's rotation at that moment
	DoorRotation.Yaw = CurrentYaw;																			// The yaw is replaced with the new value from lerping
	GetOwner()->SetActorRotation(DoorRotation);																// The rotator now overwrites the actor's rotation aka updating it

}

