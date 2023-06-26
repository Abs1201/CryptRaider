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

void UGrabber::Grab()
{
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
		AActor* HittedActor = HitResult.GetActor();
		FString HittedActorString = HittedActor->GetActorNameOrLabel();
		UE_LOG(LogTemp, Display, TEXT("Hitted actor: %s"), *HittedActorString);
	}
	else{
		UE_LOG(LogTemp, Display, TEXT("no actor found"));
	}
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
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

	
}
