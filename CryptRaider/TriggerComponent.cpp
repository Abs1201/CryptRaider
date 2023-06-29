// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    AcceptableActorTag = "Unlock1";
}

void UTriggerComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    AActor* AcceptableActor = GetAcceptableActor();
    if(AcceptableActor){
        UE_LOG(LogTemp, Warning, TEXT("open with %s"), *(AcceptableActor->GetActorNameOrLabel()));
    }
    else{
        UE_LOG(LogTemp, Display, TEXT("Relocking"));
    }

}

AActor* UTriggerComponent::GetAcceptableActor() const{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);

    for(AActor* Actor: Actors){
        if(Actor->ActorHasTag(AcceptableActorTag)){
            return Actor;
        }
    }
    return nullptr;
}
  