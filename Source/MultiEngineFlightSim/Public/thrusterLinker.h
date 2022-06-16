// Fill out your copyright notice in the Description page of Project Settings.



#pragma once



#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
#include "Components/ChildActorComponent.h"




#include "thrusterLinker.generated.h"




UCLASS(Blueprintable)
class MULTIENGINEFLIGHTSIM_API AthrusterLinker : public AActor
{
	GENERATED_BODY()
	



public:	
	// Sets default values for this actor's properties
	AthrusterLinker();

   enum Status
   {
      Right     UMETA(DisplayName = "Right side of ship"),
      Left      UMETA(DisplayName = "Left side of ship"),
      Center   UMETA(DisplayName = "Center axis of ship")
   };

   enum IsSynchronousUnit
   {
      IndependentThrusters     UMETA(DisplayName = "Each thruster in the module will act independently"),
      SingleUnit     UMETA(DisplayName = " Each thruster will be consider as one synchronous unit, ")
   };




   void BindPhysicsThruster(
      AActor* MainParentActorToBind,
      UPhysicsThrusterComponent* forwardThrusterToBind,
      UPhysicsThrusterComponent* forwardReverseThrusterToBind,
      UPhysicsThrusterComponent* upThrusterToBind,
      UPhysicsThrusterComponent* upThrusterReverseToBind,
      UPhysicsThrusterComponent* PushSideThrusterToBind,
      bool areReverseThrustersAlsoStabilierToBind, 
      enum Status sideStatus, 
      enum IsSynchronousUnit UnitResponseToBind);



   UPhysicsThrusterComponent const& getUnmodifiableReferenceToThruster() { return *forwardThrusterParent; }



   UFUNCTION(BlueprintCallable, Category = "Init")
      UPhysicsThrusterComponent* getPointerToThruster() { return  upThruster; }



   //These are temporary //
   UFUNCTION(BlueprintCallable, Category = "Init")
      void setThrusterStrengthTest(float valueTest);
   UFUNCTION(BlueprintCallable, Category = "Init")
      void setThrusterDirectionTest(float percentvalulePitch, float percentvalueForYaw) {};
   //These are temporary //




   UFUNCTION(BlueprintCallable, Category = "ThrusterInteraction")
      void setForwardReverseThrusterStrength(float percentage) { desiredThrusterForwardPercentOutput = percentage; }
   UFUNCTION(BlueprintCallable, Category = "ThrusterInteraction")
      void setUpReverseThrusterStrength(float percentage) { desiredThrusterUpPercentOutput = percentage; }
   UFUNCTION(BlueprintCallable, Category = "ThrusterInteraction")
      void setPushThrusterStrength(float percentage) { desiredThrusterPushSidePercentOutput = percentage; }



   UFUNCTION(BlueprintCallable, Category = "ThrusterInteraction")
      void setThrusterPercentPitch(float percentage) { desiredThursterPercentPitch = percentage; }
   UFUNCTION(BlueprintCallable, Category = "ThrusterInteraction")
      float getCurrentThrusterPercentPitch() { return currentThrusterPercentPitchDifference; }




   UFUNCTION(BlueprintCallable, Category = "ThrusterInteraction")
      float getForwardReverseThrusterStrength() {return desiredThrusterForwardPercentOutput; }
   UFUNCTION(BlueprintCallable, Category = "ThrusterInteraction")
      float getUpReverseThrusterStrength() { return desiredThrusterUpPercentOutput; }
   UFUNCTION(BlueprintCallable, Category = "ThrusterInteraction")
      float getCurrentPushSideThrusterStrength() { return desiredThrusterPushSidePercentOutput; }

   UFUNCTION(BlueprintCallable, Category = "ThrusterInteraction")
      float getThrusterPercentPitch() { return desiredThursterPercentPitch; }



   UFUNCTION(BlueprintCallable, Category = "ThrusterInteraction")
      float getCurrentForwardReverseThrusterPercentDiffernece() { return  currentThrusterForwardPercentDifference; }
   UFUNCTION(BlueprintCallable, Category = "ThrusterInteraction")
      float getCurrentUpReverseThrusterPercentDiffernece() { return currentThrusterUpPercentDifference; }
   UFUNCTION(BlueprintCallable, Category = "ThrusterInteraction")
      float getCurrentPushThrusterPercentDiffernece() { return currentThrusterPushSidePercentDifference; }





   UFUNCTION(BlueprintCallable, Category = "SystemSyncedTick")
      void SystemSyncedTick();// This must be exposed to blueprint callable inorder for the degalate to call it

   UFUNCTION(BlueprintCallable)
      void testingLinking();




   AActor* MainParentActor = nullptr;
   UPhysicsThrusterComponent* forwardThrusterParent = nullptr;
   UPhysicsThrusterComponent* forwardReverseThruster = nullptr;
   UPhysicsThrusterComponent* upThruster = nullptr;
   UPhysicsThrusterComponent* upReverseThruster = nullptr;
   UPhysicsThrusterComponent* PushSideThruster = nullptr;




   //TSharedPtr<UPhysicsThrusterComponent*> mastereSharedUnit = ShareThis;


   //UPhysicsThrusterComponent* isASingleUnitRef = &isASingleUnit;



private:	

   float desiredThursterPercentPitch = 0;
   float currentThrusterPercentPitchDifference = 0;

   float desiredThrusterForwardPercentOutput = 0;
   float currentThrusterForwardPercentDifference =0;

   float desiredThrusterPushSidePercentOutput = 0;
   float currentThrusterPushSidePercentDifference = 0;


   float desiredThrusterUpPercentOutput = 0;
   float currentThrusterUpPercentDifference = 0;

   float flipSignBasedOnSide = 0;



   enum IsSynchronousUnit UnitResponse;
   
   const float MAX_PITCH_ANGLE = 60.0;
   const float MIN_PITCH_ANGLE = -60.0;
   const float MAX_FORWARD_THRUST =   3000000;
   const float MAX_UP_THRUST =        3000000;
   const float MAX_PUSH_SIDE_THRUST = 1000000;
   const float MAX_REVERSE_THRUSTER_PERCENT_PARASITIC_DRAW = .35;


   bool areReverseThrustersAlsoStabilizer;
   





   void bindDelegate();
   void updateRequestedThrusterInformation();
   bool doesParentHaveVaildRoot(AActor* MainParentActor);





protected:
   // Called when the game starts or when spawned



   virtual void BeginPlay() override;

};
