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

	if (!PhysicsHandle) { return; }
	//If physicshandle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		//move the object we are holding
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
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
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	//If we hit something then attach the physics handle
	AActor* ActorHit = HitResult.GetActor();

	if (ActorHit)
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetPlayersReach());
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandle()
{
	//Check for the physicshandle component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing PhysicsHandleComponent"), *GetOwner()->GetName());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	//Raycast
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, GetPlayersWorldPos(), GetPlayersReach(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	return Hit;
}

FVector UGrabber::GetPlayersReach() const
{
	FVector POVLocation;
	FRotator POVRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT POVLocation, OUT POVRotation);

	//Draw line

	return  POVLocation + POVRotation.Vector() * Reach;
}

FVector UGrabber::GetPlayersWorldPos() const
{
	//get player's viewpoint
	FVector POVLocation;
	FRotator POVRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT POVLocation, OUT POVRotation);

	return POVLocation;
}