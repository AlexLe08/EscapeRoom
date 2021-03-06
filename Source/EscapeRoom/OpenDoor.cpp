// Copyright Alex Le 2019


#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
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

	InitialYaw = GetOwner()->GetActorRotation().Yaw;														//Starting yaw in the world; it could be 0 it could be 270 etc
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;																			//Door turns by adding to the initial yaw over time, eventually rotating from its oroginal yaw by x degrees, which is set as an editable parameter in the editor
	
	FindPressurePlate();
	FindAudioComponent();
}

void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has OpenDoor component attached but no PressurePlate set!"), *GetOwner()->GetName() );
	}
}


void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Missing audio component"), *GetOwner()->GetName());
	}

}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);											// Delta time is the time between frames being rendered

	if (TotalMassOfActors() >= MassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();														// GetWorld()->GetTimeSeconds() returns time that has passed since game started
	}
	else if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)								//  Since ^that function updates every frame, we can call (the current time - DoorLastOpened), which never changes unless its being opened, 
	{																										//  to get the difference in time, then compare that difference to DoorCloseDelay to close the door after it surpasses the delay amount
		CloseDoor(DeltaTime);
	}
	
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	// Door rotates but it rotates from an absolute position instead of relative
	// float StartingYaw = GetOwner()->GetActorRotation().Yaw;												// Gets the actor's rotation, wherever it is in the world; it could be 0 iot could be 270 who knows
	// FRotator OpenDoor = FRotator(0.0f,OpenAngle,0.0f);

	// Lerp(StartingValue, TargetValue, value bnetween 0 - 1.0f)											// Lerp is linear interpolation, or how fast a number changes from its start to target

	// OpenDoor.Yaw = FMath::Lerp(StartingYaw, OpenAngle, 0.1f);											// Every frame the rotator gets a new starting yaw,
	// GetOwner()->SetActorRotation(OpenDoor);																// Which then gets set as the actor's new rotation

	// Better implementation //
	//UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DoorOpenSpeed * DeltaTime);								// Current yaw is set each frame to a new value that eventually reaches the desired OpenAngle, 
	//																										// ALSO we multiple by Delta time to make the change over time independent from frame rates so the velocity is constant on slow and fast PCS

	FRotator DoorRotation = GetOwner()->GetActorRotation();													// A rotator is created and assigned to be the actor's rotation at that moment
	DoorRotation.Yaw = CurrentYaw;																			// The yaw is replaced with the new value from lerping
	GetOwner()->SetActorRotation(DoorRotation);

	CloseDoorSound = false;
	if (!AudioComponent) {return;}
	if (!OpenDoorSound)
	{
		AudioComponent->Play();
		OpenDoorSound = true;
	}

}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("Door is closing"));
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DoorCloseSpeed * DeltaTime);							// Same code as OpenDoor, but instead of lerping to OpenAngle, we go back to InitialYaw.

	FRotator DoorRotation = GetOwner()->GetActorRotation();													
	DoorRotation.Yaw = CurrentYaw;		
	GetOwner()->SetActorRotation(DoorRotation);	

	OpenDoorSound = false;
	if (!AudioComponent) {return;}	
	if (!CloseDoorSound)
	{
		AudioComponent->Play();
		CloseDoorSound = true;
	}										
}

// float UOpenDoor::TotalMassOfActors() const																	// Returns the total mass of all actors that overlap the pressure plate
// {
// 	float TotalMass = 0.f;

// 	TArray<AActor*> OverlappingActors;
// 	if (!PressurePlate) {return TotalMass;}
// 	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

// 	for (AActor* Actor : OverlappingActors)
// 	{
// 		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
// 		UE_LOG(LogTemp, Warning, TEXT("%s is on the pressure plate"), *Actor->GetName());
// 	}

// 	return TotalMass;
// }

float UOpenDoor::TotalMassOfActors() const																	// Returns the total mass of all actors that overlap the pressure plate
{
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) {return TotalMass;}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s is on the pressure plate"), *Actor->GetName());
	}

	return TotalMass;
}
