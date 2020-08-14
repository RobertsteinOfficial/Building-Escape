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

	//FString Log = TEXT("Hello");
	//FString *PtrLog = &Log;

	//Log.Len();
	//PtrLog->Len();

	//UE_LOG(LogTemp, Warning, TEXT("%s"), **PtrLog);		//devo usare due asterischi perch� per le stringhe * � overloaded, 
	//														//quindi in realt� il primo � il pointer, il secondo � l'overloaded operator

	FString ObjName = GetOwner()->GetName();
	UE_LOG(LogTemp, Warning, TEXT("The name is %s"), *GetOwner()->GetName());
	FString ObjPosition = GetOwner()->GetActorLocation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("The Location is %s"), *ObjPosition);
}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

