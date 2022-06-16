// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ExternalFiles/psocpp.h"


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
#include "Components/ChildActorComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Templates/SubclassOf.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "AssetRegistryModule.h"
#include "Kismet/GameplayStatics.h"
#include "Containers/Array.h"



#include "thrusterLinker.h"
#include "BetaShip.h"
#include "EngineActor.h"
#include "neuralNetworkOptimization.h"


#include "flightinterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNeewTypeDelgate);


//template <typename T>
////UCLASS(Blueprintable)
//class   Ainside : public UObject
//{
//
//   GENERATED_BODY()
//public:
//   UFUNCTION(BlueprintCallable)
//      T* YourFunction(T* passing);
//
//
//   this* tempin;
//
//
//   UFUNCTION(BlueprintCallable)
//      AActor* getaInside() { return tempin; }
//};

USTRUCT(BlueprintType)
struct FTargetSessionValues
{
   GENERATED_BODY()

      //~ The following member variable will be accessible by Blueprint Graphs:
      // This is the tooltip for our test variable.
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Stablization")
      float target_Pitch = 0;
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Stablization")
      float target_Yaw = 0;
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Stablization")
      float target_Roll = 0;





   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Allowed DeadSpace Values")
      float Pitch_DeadSpace = 0;
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Allowed DeadSpace Values")
      float Yaw_DeadSpace = 0;
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Allowed DeadSpace Values")
      float Roll_DeadSpace = 0;




   // These Vaules will be constant values, do not change.
   // in percent current thruster range is -60 degress to 60 degrees.
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Allowed DeadSpace Values")
      float frontRightEnginePercentPitch = 70;
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Allowed DeadSpace Values")
      float frontLeftEnginePercentPitch = 70;
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Allowed DeadSpace Values")
      float rearUpperLeftEnginePercentPitch = -25;
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Allowed DeadSpace Values")
      float rearUpperRightEnginePercentPitch = -25;
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Allowed DeadSpace Values")
      float rearLowerLeftEnginePercentPitch = 70;
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Allowed DeadSpace Values")
      float RearLowerRightEnginePercentPitch = 70;



   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target LOCk Loaction")
      FVector targetLocationLock = FVector(0.0f, 0.0f, 0.0f);

};


USTRUCT(BlueprintType)
struct FflightData
{
   GENERATED_BODY()

   UPROPERTY(BlueprintReadOnly, Category = "Debug Flight Data ")
      float xAxisRotationVelocity;
   UPROPERTY(BlueprintReadOnly, Category = "Debug Flight Data ")
      float yAxisRotationVelocity;
   UPROPERTY(BlueprintReadOnly, Category = "Debug Flight Data ")
      float zAxisRotationVelocity;


   UPROPERTY(BlueprintReadOnly, Category = "Debug Flight Data ")
      float xAxisVelocity;
   UPROPERTY(BlueprintReadOnly, Category = "Debug Flight Data ")
      float yAxisVelocity;
   UPROPERTY(BlueprintReadOnly, Category = "Debug Flight Data ")
      float zAxisVelocity;


   UPROPERTY(BlueprintReadOnly, Category = "Debug Flight Data ")
      float xAxisAngle;
   UPROPERTY(BlueprintReadOnly, Category = "Debug Flight Data ")
      float yAxisAngle;
   UPROPERTY(BlueprintReadOnly, Category = "Debug Flight Data ")
      float zAxisAngle;


   UPROPERTY(BlueprintReadOnly, Category = "Debug Flight Data ")
      float xAxisAcceleration;
   UPROPERTY(BlueprintReadOnly, Category = "Debug Flight Data ")
      float yAxisAcceleration;
   UPROPERTY(BlueprintReadOnly, Category = "Debug Flight Data ")
      float zAxisAcceleration;




   UPROPERTY(BlueprintReadOnly, Category = "Flight Data ")
      FVector worldSpaceLocation = FVector(0.0f, 0.0f, 0.0f);
   UPROPERTY(BlueprintReadOnly, Category = "Flight Data ")
      FQuat worldSpaceRotationQuat = FQuat(0.0f, 0.0f, 0.0f, 0.0f);


   // NOT TO BE USED IN FINAL CODE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   UPROPERTY(BlueprintReadOnly, Category = "Debug ONLY")
      float Debug_X_Axis_Current_Offset = 0;
   UPROPERTY(BlueprintReadOnly, Category = "Debug ONLY")
      float Debug_Y_Axis_Current_Offset = 0;
   UPROPERTY(BlueprintReadOnly, Category = "Debug ONLY")
      float Debug_Z_Axis_Current_Offset = 0;
   // NOT TO BE USED IN FINAL CODE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



   UPROPERTY(BlueprintReadOnly, Category = "Debug Flight Data ")
      int64 altitude;


   UPROPERTY(BlueprintReadOnly, Category = "Debug Flight Data ")
      FVector initalTickVelocity_AcclerationCalulation; // Not used to retreve veloicty of each indivual axis.
   UPROPERTY(BlueprintReadOnly, Category = "Debug Flight Data ")
      FVector PreviousTickVelocity_AcclerationCalulation; // Not used to retreve veloicty of each indivual axis.

};



USTRUCT(BlueprintType)
struct FFlightPathAggressionTemplate
{
   GENERATED_BODY()


   /*
    * Stage 1
    * This will take the craft up to traveling speed
   * The craft will be at target traveling speeds, a acceleration of zero and within its
   * Max travel Velocity
    */
  //UPROPERTY(BlueprintReadOnly, Category = "Debug Flight Data ")
      //float endLowerBoundDistanceToTarget;
   UPROPERTY(BlueprintReadOnly, Category = "Debug Flight Data ")
      float MaxVelocityLowerBound;
   UPROPERTY(BlueprintReadOnly, Category = "Debug Flight Data ")
      float MaxAccelerationLowerBound;

   /*
    * Stage 2
    * This will take the craft from traveling veloiccty and begin to slow down 
    * to a target velocity of Zero and a target acceleration of Zero
    * The goal of this sectional conditions is to take the craft to a stand still 
    * At what ever the target point is.
    */

   UPROPERTY(BlueprintReadOnly, Category = "Distnace to Zero Target Point")
      float StartUpperBoundDistance;
   // Target velocity == 0
   // Target Location == 0

   //UPROPERTY(BlueprintReadOnly, Category = "Debug Flight Data ")
   //   float yAxisAngle;
   //UPROPERTY(BlueprintReadOnly, Category = "Debug Flight Data ")
   //   float zAxisAngle;

   //TODO////
   /*
   * Creat a calculation that will make a minimum required distance to start 
   * and stop based on the velocity of the craft
   */




};


UCLASS(Blueprintable)
class  Aflightinterface : public AActor 
{
	GENERATED_BODY()


public:	



	// Sets default values for this actor's properties
	Aflightinterface(const FObjectInitializer& ObjectInitializer);


   /************************************/
   //SCOPED STRCUT DATA



   UPROPERTY(BlueprintReadWrite, EditAnywhere)
      int32 returned_Pitch;
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
      int32 returned_Yaw;
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
      int32 returned_Roll;
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
      int64 returned_Altitude;
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
      int64 returned_Elivation;



   UPROPERTY(BlueprintCallable, BlueprintAssignable)
      FNeewTypeDelgate myNewDelegate;






   UFUNCTION(BlueprintCallable)
      void BradcastDelegateToWorld(); // This must be exposed to blueprint callable inorder for the degalate to call it

   UFUNCTION(BlueprintCallable, Category = "SystemSyncedTick")
      void SystemSyncedTick();// This must be exposed to blueprint callable inorder for the degalate to call it




   UPROPERTY(BlueprintReadWrite, EditAnywhere)
      float maxUpVelocity;

   UFUNCTION(BlueprintCallable, Category = "Init")
      bool InitThrusterTest();

   UFUNCTION(BlueprintCallable, Category = "Init")
      void bindRequiredComponents(
         UPhysicsThrusterComponent* FFR,
         UPhysicsThrusterComponent* FFL,
         UPhysicsThrusterComponent* FRR,
         UPhysicsThrusterComponent* FRL,

         UPhysicsThrusterComponent* RFR,
         UPhysicsThrusterComponent* RFL,
         UPhysicsThrusterComponent* RRR,
         UPhysicsThrusterComponent* RRL,

         UPhysicsThrusterComponent* URRM,
         UPhysicsThrusterComponent* URRMrev,
         UPhysicsThrusterComponent* URRU,
         UPhysicsThrusterComponent* URRUrev,
         UPhysicsThrusterComponent* URRMPR,

         UPhysicsThrusterComponent* URLM,
         UPhysicsThrusterComponent* URLMrev,
         UPhysicsThrusterComponent* URLU,
         UPhysicsThrusterComponent* URLUrev,
         UPhysicsThrusterComponent* URLMPR,

         UPhysicsThrusterComponent* LRRM,
         UPhysicsThrusterComponent* LRRMrev,
         UPhysicsThrusterComponent* LRRU,
         UPhysicsThrusterComponent* LRRUrev,
         UPhysicsThrusterComponent* LRRMPR,

         UPhysicsThrusterComponent* LRLM,
         UPhysicsThrusterComponent* LRLMrev,
         UPhysicsThrusterComponent* LRLU,
         UPhysicsThrusterComponent* LRLUrev,
         UPhysicsThrusterComponent* LRLMPR,

         UPhysicsThrusterComponent* FRM,
         UPhysicsThrusterComponent* FRMrev,
         UPhysicsThrusterComponent* FRU,
         UPhysicsThrusterComponent* FRUrev,
         UPhysicsThrusterComponent* FRMPR,

         UPhysicsThrusterComponent* FLM,
         UPhysicsThrusterComponent* FLMrev,
         UPhysicsThrusterComponent* FLU,
         UPhysicsThrusterComponent* FLUrev,
         UPhysicsThrusterComponent* FLMPR,

         UChildActorComponent* frontRightEngineModule,
         UChildActorComponent* frontLeftEngineModule,
         UChildActorComponent* upperRearRightEngineModule,
         UChildActorComponent* upperRearLeftEngineModule,
         UChildActorComponent* lowerRearRightEngineModule,
         UChildActorComponent* lowerRearLeftEngineModule,

         USceneComponent* targetMeshComponet);

   UFUNCTION(BlueprintCallable, Category = "Init")
      void runInitRotationControl();







   UPROPERTY(EditAnywhere, BlueprintReadWrite)
      TSubclassOf<AActor> myBPClass;




   /* THRUSTERS LINKER SYSTEMS */

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
      AthrusterLinker* singleFrontModule;
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
      AthrusterLinker* singleRearModule;

   UPROPERTY(BlueprintReadWrite, EditAnywhere)
      AthrusterLinker* upperRearRightMainThruster;
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
      AthrusterLinker* upperRearLeftMainThruster;

   UPROPERTY(BlueprintReadWrite, EditAnywhere)
      AthrusterLinker* lowerRearRightMainThruster;
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
      AthrusterLinker* lowerRearLeftMainThruster;
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
      AthrusterLinker* frontRightMainThruster;
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
      AthrusterLinker* frontLeftMainThruster;

   /* THRUSTERS LINKER SYSTEMS */



   AActor* frontRightEngineActor = nullptr;
   AActor* frontLeftEngineActor = nullptr;
   AActor* upperRearRightEngineActor = nullptr;
   AActor* upperRearLeftEngineActor = nullptr;
   AActor* lowerRearRightEngineActor = nullptr;
   AActor* lowerRearLeftEngineActor = nullptr;



   //TSharedRef<Aflightinterface, ESPMode::ThreadSafe> testpointer;

   FflightData* FlightData; // flight data stuct dec
   FFlightPathAggressionTemplate* FlightPathAggressionStruct;
   FTargetSessionValues* TargetSession;  // <==== NEED TO EXPOSE TO BLUEPRINTS


   UFUNCTION(BlueprintCallable, Category = "Debug")
      FflightData getFlightData() { return *FlightData;}

   UFUNCTION(BlueprintCallable, Category = "Debug")
      FFlightPathAggressionTemplate getFlightPathAggressionTemplate() { return *FlightPathAggressionStruct; }

   UFUNCTION(BlueprintCallable, Category = "Debug")
      FTargetSessionValues getFlightSessionValues() { return *TargetSession; }


private:

   float tempstate = 0;

   // Side thrusters percent dividers
   void sideThrustersSidePercentDistribution(float allotted_thrust_percentage /* negativee means push to the left, positive means push to the right */, float alloted_DeadSpace);

   //void runEqulizer();

   void parentMeshAxisValues(USceneComponent* targetMeshforvelocity);


   // START Gaia Stabilization System Module //

   // Directional Lock Systems Moduels
   void targetLocationLock();      // WIP

   // Rotational Lock system Moduels
   void pitchLockStabilization(float current_craft_pitch, float target_craft_pitch, float allotted_thrust_percentage,  float alloted_DeadSpace);      // TODO
   void rollLockStabilization();    // TODO
   void yawLockStabilization();     // TODO

   // Lock To Target Acceleration
   float setTargetAcceleration_Axis_PercentStepSize(float target_Acceleration, float current_Acceleration, const float aggression_Curve /* Lower is smoother but slower */ , float const allowed_DeadSpace); // Complete

   // Find Deceleration Fucntion
   float getTargetAccelerationBasedOnGoalWorldLocation(float current_Velocity, float target_Z_Axis_Goal, float craft_Current_Location, float Asked_Time_To_Stop); // Complete



   // END Gaia Stabilization System Module //





   // Bind Globle Gaia Tick System
   void BindDelegate();




   AneuralNetworkOptimization* NNO;

   USceneComponent* parentMesh;

   AActor* objectOwner; // Who owns Parent Mesh

   // Used For acceleration calculation due to the fact that this value
   // Cant be aquired directly through the Game Engine
   FVector currentWorldDeltaTimeInVectorForm ;
   FVector currentAllAxisAccleration;






protected:
   // Called when the game starts or when spawned
   virtual void BeginPlay() override;

};
