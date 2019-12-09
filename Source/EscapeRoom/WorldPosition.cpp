// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPosition.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	// ...
	// FString Log = TEXT("Hello");
	// FString* PrtLog = &Log;

	// Log.Len();
	// 													SAME AS (without the extra step of using a copy)
	// (*PrtLog).Len(); 								OR
	// PrtLog->Len();

	// UE_LOG(LogTemp, Warning, TEXT("%s"), **PrtLog);		** is dereferencing then overloading to get the FString within


	FString ObjName = GetOwner()->GetName();
	UE_LOG(LogTemp, Warning, TEXT("Object Name is: %s"), *ObjName);					//* is overloading

	//Can also be done like this to save lines; 
	// save to a variable if its gonna be used around the code
	//UE_LOG(LogTemp, Warning, TEXT("Object Name is: %s"),  *GetOwner()->GetName());

	FString ObjPosition = GetOwner()->GetActorLocation().ToString();
	UE_LOG(LogTemp, Display, TEXT("%s is at the position: %s"), *ObjName, *ObjPosition);
	
}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

