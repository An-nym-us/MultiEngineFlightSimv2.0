// Fill out your copyright notice in the Description page of Project Settings.



#include "neuralNetworkOptimization.h"

// THIS MUST STAY HERE< THIS WILL PREVENT CIRULARE DEPENDENCIES
#include "flightinterface.h" 
// THIS MUST STAY HERE< THIS WILL PREVENT CIRULARE DEPENDENCIES
// 
// 
// 
// Sets default values
AneuralNetworkOptimization::AneuralNetworkOptimization()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AneuralNetworkOptimization::BeginPlay()
{
	Super::BeginPlay();
   BindDelegate();

}
void AneuralNetworkOptimization::SystemSyncedTick()
{



}



void AneuralNetworkOptimization::BindDelegate()
{
   Aflightinterface* temp = Cast<Aflightinterface>(GetOwner());
   if (temp)
   {
      temp->myNewDelegate.AddDynamic(this, &AneuralNetworkOptimization::SystemSyncedTick);
   }
}




