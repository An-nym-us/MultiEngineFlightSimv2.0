// Fill out your copyright notice in the Description page of Project Settings.

#include "thrusterLinker.h"


// THIS MUST STAY HERE< THIS WILL PREVENT CIRULARE DEPENDENCIES
#include "flightinterface.h" 
// THIS MUST STAY HERE< THIS WILL PREVENT CIRULARE DEPENDENCIES






// Sets default values
AthrusterLinker::AthrusterLinker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

   // Init variables




}

// Called when the game starts or when spawned
void AthrusterLinker::BeginPlay()
{
	Super::BeginPlay();
   bindDelegate();

}


void AthrusterLinker::SystemSyncedTick()
{

   updateRequestedThrusterInformation();


}



void AthrusterLinker::bindDelegate()
{
   Aflightinterface* temp = Cast<Aflightinterface>(GetOwner());
   if (temp)
   {
      temp->myNewDelegate.AddDynamic(this, &AthrusterLinker::SystemSyncedTick);
   }
}



void AthrusterLinker::testingLinking()
{
   //if(PushSideThruster)
      //PushSideThruster->ThrustStrength = 777777767;
}


bool AthrusterLinker::doesParentHaveVaildRoot(AActor* MainParentActorCheck)
{
   if (MainParentActorCheck)
   {
      USceneComponent* BrainDead = MainParentActorCheck->GetRootComponent();
      if (BrainDead)
         return true;
      else
         return false;
   }
   else
   {
      return false;
   }
}


void AthrusterLinker::updateRequestedThrusterInformation()
{
   // Only run this section of code assuming the unit is a thruster Module


   // adjust angle of the whole thruster Unit
   if (doesParentHaveVaildRoot(MainParentActor) && UnitResponse == IsSynchronousUnit::IndependentThrusters)
   {
      // Access Main Parent Root Component for rotation.
      USceneComponent* root = MainParentActor->GetRootComponent();



      float X = UKismetMathLibrary::FClamp(desiredThursterPercentPitch, -100, 100);
      float V = UKismetMathLibrary::NormalizeToRange(X, -100, 100);
      float requestedPitch = UKismetMathLibrary::Lerp(MIN_PITCH_ANGLE, MAX_PITCH_ANGLE, V);


      float XX = UKismetMathLibrary::NormalizeToRange(root->GetRelativeRotation().Pitch, -60, 60);
      float VV = UKismetMathLibrary::Lerp(-100, 100, XX);
      float CurrentPitch = UKismetMathLibrary::FClamp(VV, -100, 100);
      
      currentThrusterPercentPitchDifference = desiredThursterPercentPitch - CurrentPitch ;


      FRotator CurrentRotation = root->GetRelativeRotation();
      FRotator targetrootation = FRotator(requestedPitch, 0, 0);
      float sensitivity = .005;

      //forwardThrusterParent->SetRelativeRotation(UKismetMathLibrary::DynamicWeightedMovingAverage_FRotator(targetrootation, CurrentRotation, NULL, sensitivity, NULL));
      root->SetRelativeRotation(UKismetMathLibrary::WeightedMovingAverage_FRotator(targetrootation, CurrentRotation, sensitivity));

      //UE_LOG(LogTemp, Warning, /*TEXT("XR: %f"),*/ TEXT("%f"), currentThrusterPercentPitchDifference);
   }


   // forward and reverse thrsuters
   if (forwardThrusterParent && forwardReverseThruster && UnitResponse == IsSynchronousUnit::IndependentThrusters)
   {

      float X = UKismetMathLibrary::FClamp(desiredThrusterForwardPercentOutput, -100, 100);
      float V = UKismetMathLibrary::NormalizeToRange(X, 0, 100);
      float requestedForwardThrust = UKismetMathLibrary::Lerp(0, MAX_FORWARD_THRUST, V);



      float XX = UKismetMathLibrary::NormalizeToRange(forwardThrusterParent->ThrustStrength, 0, MAX_FORWARD_THRUST);
      float VV = UKismetMathLibrary::Lerp(-100, 100, XX);
      float CurrentThrust = UKismetMathLibrary::FClamp(VV, -100, 100);



      currentThrusterForwardPercentDifference = desiredThrusterForwardPercentOutput - CurrentThrust;

      //UE_LOG(LogTemp, Warning, /*TEXT("XR: %f"),*/ TEXT("%f"), V);


      if (currentThrusterForwardPercentDifference >= 0)
      {  
         forwardThrusterParent->ThrustStrength = requestedForwardThrust;
         forwardReverseThruster->ThrustStrength = 0;

      }
      else if (currentThrusterForwardPercentDifference < 0 )
      {
         forwardThrusterParent->ThrustStrength = 0;
         forwardReverseThruster->ThrustStrength = requestedForwardThrust *.35;
      }

   }



   // Side Thrusters
   if (PushSideThruster && UnitResponse == IsSynchronousUnit::IndependentThrusters)
   {

      float X = UKismetMathLibrary::FClamp(desiredThrusterPushSidePercentOutput, 0, 100);
      float V = UKismetMathLibrary::NormalizeToRange(X, 0, 100);
      float requestedSidePushThrust = UKismetMathLibrary::Lerp(0, MAX_PUSH_SIDE_THRUST, V);



      float XX = UKismetMathLibrary::NormalizeToRange(PushSideThruster->ThrustStrength, 0, MAX_PUSH_SIDE_THRUST);
      float VV = UKismetMathLibrary::Lerp(0, 100, XX);
      float CurrentSidePushThrust = UKismetMathLibrary::FClamp(VV, 0, 100);

      currentThrusterPushSidePercentDifference = desiredThrusterPushSidePercentOutput - CurrentSidePushThrust;

      
      //UE_LOG(LogTemp, Warning, /*TEXT("XR: %f"),*/ TEXT("%f"), desiredThrusterPushSidePercentOutput);
      PushSideThruster->ThrustStrength = requestedSidePushThrust; // Set the request thurster strength.

      //PushSideThruster->Activate();
   }




   if (upThruster && upReverseThruster && UnitResponse == IsSynchronousUnit::IndependentThrusters)
   {

      float X = UKismetMathLibrary::FClamp(desiredThrusterUpPercentOutput, -100, 100);
      float V = UKismetMathLibrary::NormalizeToRange(X, 0, 100);
      float requestedUpThrust = UKismetMathLibrary::Lerp(0, MAX_UP_THRUST, V);



      float XX = UKismetMathLibrary::NormalizeToRange(upThruster->ThrustStrength, 0, MAX_UP_THRUST);
      float VV = UKismetMathLibrary::Lerp(-100, 100, XX);
      float CurrentThrust = UKismetMathLibrary::FClamp(VV, -100, 100);





      //currentThrusterUpPercentDifference = desiredThrusterUpPercentOutput - CurrentThrust;




      if (desiredThrusterUpPercentOutput >= 0)
      {
         upThruster->ThrustStrength = requestedUpThrust;
         upReverseThruster->ThrustStrength = 0;
         //UE_LOG(LogTemp, Warning, /*TEXT("XR: %f"),*/ TEXT("%f"), upThruster->ThrustStrength);

      }
      else if (desiredThrusterUpPercentOutput < 0)
      {
         upThruster->ThrustStrength = 0;
         upReverseThruster->ThrustStrength = requestedUpThrust * .35;
        // UE_LOG(LogTemp, Warning, /*TEXT("XR: %f"),*/ TEXT("%f"), upReverseThruster->ThrustStrength);
      }


   }






   if (UnitResponse == IsSynchronousUnit::SingleUnit && true /* this true statement will be repaced with an enum asking of the thrusters are staitic in movement */)
   {



      float X = UKismetMathLibrary::FClamp(desiredThrusterForwardPercentOutput, -100, 100);
      float V = UKismetMathLibrary::NormalizeToRange(X, 0, 100);
      float requestedUpThrust = UKismetMathLibrary::Lerp(0, MAX_UP_THRUST, V);


      if(forwardThrusterParent)
         forwardThrusterParent->ThrustStrength = requestedUpThrust;
      if(forwardReverseThruster)
         forwardReverseThruster->ThrustStrength = requestedUpThrust;
      if(upThruster)
         upThruster->ThrustStrength = requestedUpThrust;
      if(upReverseThruster)
         upReverseThruster->ThrustStrength = requestedUpThrust;
      if(PushSideThruster)
         PushSideThruster->ThrustStrength = requestedUpThrust;



   }

}



void AthrusterLinker::BindPhysicsThruster(
   AActor* MainParentActorToBind,
   UPhysicsThrusterComponent* forwardThrusterToBind,
   UPhysicsThrusterComponent* forwardReverseThrusterToBind,
   UPhysicsThrusterComponent* upThrusterToBind,
   UPhysicsThrusterComponent* upReverseThrusterToBind,
   UPhysicsThrusterComponent* PushSideThrusterToBind,
   bool areReverseThrustersAlsoStabilierToBind,
   enum Status sideStatus,
   enum IsSynchronousUnit UnitResponseToBInd
)
{
   
   UnitResponse = UnitResponseToBInd;


   if (UnitResponse == IsSynchronousUnit::IndependentThrusters)
   {
      MainParentActor = MainParentActorToBind;
      forwardThrusterParent = forwardThrusterToBind;
      forwardReverseThruster = forwardReverseThrusterToBind;
      upThruster = upThrusterToBind;
      upReverseThruster = upReverseThrusterToBind;
      PushSideThruster = PushSideThrusterToBind;

      areReverseThrustersAlsoStabilizer = areReverseThrustersAlsoStabilierToBind;
   }
   else if (UnitResponse == IsSynchronousUnit::SingleUnit)
   {
      // Impliment a shared pointer system
      
      forwardThrusterParent = forwardThrusterToBind;
      forwardReverseThruster = forwardReverseThrusterToBind;
      upThruster = upThrusterToBind;
      upReverseThruster = upReverseThrusterToBind;
      PushSideThruster = PushSideThrusterToBind;

      // Impliment a shared pointer system
   }



   if (sideStatus)
   {
      if (sideStatus == Status::Right)
         flipSignBasedOnSide = 1;

      else if (sideStatus == Status::Left)
         flipSignBasedOnSide = -1;
      
      else if (sideStatus == Status::Center)
         flipSignBasedOnSide = 0;
   }


}


void AthrusterLinker::setThrusterStrengthTest(float valueTest)
{


}


