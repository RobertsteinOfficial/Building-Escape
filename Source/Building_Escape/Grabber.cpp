// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT			//é una macro che non fa un cazzo, è solo per marcare parametri out dele funzioni, giusto per leggibilità

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();

}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	//If physicshandle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		//get player's viewpoint
		FVector POVLocation;
		FRotator POVRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT POVLocation, OUT POVRotation);

		//Draw line
		FVector LineTraced = POVLocation + POVRotation.Vector() * Reach;

		//move the object we are holding
		PhysicsHandle->SetTargetLocation(LineTraced);
	}


}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab"));

	//get player's viewpoint
	FVector POVLocation;
	FRotator POVRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT POVLocation, OUT POVRotation);

	//Draw line
	FVector LineTraced = POVLocation + POVRotation.Vector() * Reach;

	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	//If we hit something then attach the physics handle
	//TODO attach physics handle

	if (HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, LineTraced);
	}

}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release"));

	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandle()
{
	//Check for the physicshandle component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{
		//non serve codice, solo per protezione
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing PhysicsHandleComponent"), *GetOwner()->GetName());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	//get player's viewpoint
	FVector POVLocation;
	FRotator POVRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT POVLocation, OUT POVRotation);

	//Draw line
	FVector LineTraced = POVLocation + POVRotation.Vector() * Reach;
	DrawDebugLine(GetWorld(), POVLocation, LineTraced, FColor(0, 255, 0), false, 0.f, 0, 5.f);

	//Raycast
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, POVLocation, LineTraced, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("I hit %s"), *(ActorHit->GetName()));
	}

	return Hit;
}