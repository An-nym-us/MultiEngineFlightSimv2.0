// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/RigidBodyBase.h"
#include "PhysicsEngine/PhysicsThruster.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "UObject/UObjectGlobals.h"
#include "Kismet/KismetMathLibrary.h"
#include "Logging/LogMacros.h"
#include "Components/PrimitiveComponent.h"
#include "Containers/UnrealString.h"
#include "Engine/World.h"



#include "neuralNetworkOptimization.generated.h"

UCLASS()
class MULTIENGINEFLIGHTSIM_API AneuralNetworkOptimization : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AneuralNetworkOptimization();


   UFUNCTION(BlueprintCallable, Category = "SystemSyncedTick")
      void SystemSyncedTick();// This must be exposed to blueprint callable inorder for the degalate to call it

private:
   // Called every frame


   void BindDelegate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
