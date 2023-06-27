// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle){
		FString HittedName = PhysicsHandle->GetName();
		UE_LOG(LogTemp, Display, TEXT("hittedName: %s"), *HittedName);

	}
	else{
		UE_LOG(LogTemp,Warning, TEXT("nullptr allocated!"));
	}
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle == nullptr){ return;}
	FVector TargetLocation = GetComponentLocation() + GetForwardVector() * GrabDistance;
	PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	// UPrimitiveComponent* GrabbedComponent = PhysicsHandle->GetGrabbedComponent();
	// GrabbedComponent->SetActorLocationAndRotation(TargetLocation, GetComponentRotation());
	
}


void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle == nullptr){ return;}
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector()*MaxGrapDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	FHitResult HitResult;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, 	
		Start, End,
		FQuat::Identity, ECC_GameTraceChannel2,
		Sphere);
	
	if(HasHit){
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->WakeAllRigidBodies();
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
		DrawDebugSphere(GetWorld(), End, 10,10,FColor::Cyan, false, 5);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Red, false, 5);
	}
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle == nullptr){ return; }
	UPrimitiveComponent* GrabbedComponent = PhysicsHandle->GetGrabbedComponent();
	if(GrabbedComponent == nullptr){return;}
	PhysicsHandle->ReleaseComponent();
}

UPhysicsHandleComponent *UGrabber::GetPhysicsHandle() const
{	
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(Result == nullptr){
		UE_LOG(LogTemp, Error, TEXT("Grabber need PhysicsHandle Component"));
	}
	return Result;
}
