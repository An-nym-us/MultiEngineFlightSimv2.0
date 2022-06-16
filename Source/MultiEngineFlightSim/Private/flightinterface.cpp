// Fill out your copyright notice in the Description page of Project Settings.

#include "flightinterface.h"


// Sets default values
Aflightinterface::Aflightinterface(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
   PrimaryActorTick.bCanEverTick = true;

   FlightData = new FflightData();
   FlightPathAggressionStruct = new FFlightPathAggressionTemplate();
   TargetSession = new FTargetSessionValues();

   // Testing target values for session. Default Values
   TargetSession->targetLocationLock.X = 600;
   TargetSession->targetLocationLock.Y = -7000;
   TargetSession->targetLocationLock.Z = 2500;



}

// Called when the game starts or when spawned
void Aflightinterface::BeginPlay()
{
	Super::BeginPlay();
	
   if(GetOwner())
      objectOwner = GetOwner();

   BindDelegate();

}



void Aflightinterface::SystemSyncedTick()
{ 
    parentMeshAxisValues(parentMesh);
    targetLocationLock();

    //Test


}



// USed for debug test to change target location mid-flight
void Aflightinterface::changeTargetFlightLoction(FVector new_target_Lock_Location)
{
   if (!TargetSession)
      return;

   TargetSession->targetLocationLock = new_target_Lock_Location;

   

}

void Aflightinterface::targetLocationLock()
{
   //Useful data to look at   
   FlightData->zAxisAcceleration;
   FlightData->zAxisVelocity;
   float stepside = .01;
   float deadSpace = .01;


   // flight bound stuct table
   
   /***********************************************************************************
   * Goal --->
   * Get craft from STAND STILL to a maximum velocity of Max Velocity
   * 
   * USING --->
   * MAX acceleration of Lower Bounds
   ***********************************************************************************/

   FlightPathAggressionStruct->MaxVelocityLowerBound = 20;
   FlightPathAggressionStruct->MaxAccelerationLowerBound = 100;


   FlightPathAggressionStruct->StartUpperBoundDistance = 1000;



   /***********************************************************************************
   * Goal --->
   * Get craft to a target velocity of Zero and to Target stop point
   * 
   * USING --->
   *  EQUAATION     
   *     {Solve for acceleleration}
   *     d = (Velocity_inital * TIME) + (.5 * Target Acceleration To Stop * (TIME ^2)) 
   * 
   * FUNCTION EQUALS
   *     Target Acceleration To Stop = (Distance - Velocity_inital * TIME) / (.5 * (TIME ^ 2))
   * 
   * Accleeration needed to stop will be a goal based function. seeing that the craft
   * will always have a verying weight, the system will have to guess and check its 
   * 
   ***********************************************************************************/


   // Ignore this stuff Debuig Only ============================================================================
   FlightData->Debug_X_Axis_Current_Offset = FlightData->worldSpaceLocation.X - TargetSession->targetLocationLock.X;
   FlightData->Debug_Y_Axis_Current_Offset = FlightData->worldSpaceLocation.Y - TargetSession->targetLocationLock.Y;
   FlightData->Debug_Z_Axis_Current_Offset = FlightData->worldSpaceLocation.Z - TargetSession->targetLocationLock.Z;
   // Ignore this stuff Debuig Only ============================================================================






   float sendTempAcceleratonX;
   float sendTempAcceleratonY;
   float sendTempAcceleratonZ;
   float Asked_Time_To_Stop = 3; // In Seconds

   float targetMaxAcceleration = 100;

 /************************************************************************************
*EACH TARGET AXIS ACCELERATION  IS INDEPENDENT TO THE ORIENTATION OF THE CRAFT!!!!
* This value will be used in future itterations of the program. Because the crafts 
* orentation is indepeneted of teh target Axis world locations, it does not 
* matter what way the craft is facing, As long as it maintains this axis 
* accleration to its target point.
************************************************************************************/

   // This is the BIG BIG BRAINS of the Gaia system. This will get the crafts acceleration (either Positive or Negative) Neededd too reach its new target location.
   // As stated this is independent to orenttation, and is only axis based. 
   float TARGET_X_AXIS_ACCELERATION = getTargetAccelerationBasedOnGoalWorldLocation(FlightData->xAxisVelocity, TargetSession->targetLocationLock.X, FlightData->worldSpaceLocation.X, Asked_Time_To_Stop);
   float TARGET_Y_AXIS_ACCELERATION = getTargetAccelerationBasedOnGoalWorldLocation(FlightData->yAxisVelocity, TargetSession->targetLocationLock.Y, FlightData->worldSpaceLocation.Y, Asked_Time_To_Stop);
   float TARGET_Z_AXIS_ACCELERATION = getTargetAccelerationBasedOnGoalWorldLocation(FlightData->zAxisVelocity, TargetSession->targetLocationLock.Z, FlightData->worldSpaceLocation.Z, Asked_Time_To_Stop);

   

   // Orentation does not matter FOR NOW!!!!
   // X-axis
   sendTempAcceleratonX = UKismetMathLibrary::FClamp(TARGET_X_AXIS_ACCELERATION, (targetMaxAcceleration * -1), targetMaxAcceleration);
   float CurrentpercentStepSizeX = setTargetAcceleration_Axis_PercentStepSize(sendTempAcceleratonX, FlightData->xAxisAcceleration, .01 /* Lower is smoother but slower */, .1);   // Acceleration Control // System Fucntions Corrently   :D <-- Happy face!
   
   // Y-axis
   sendTempAcceleratonY = UKismetMathLibrary::FClamp(TARGET_Y_AXIS_ACCELERATION, (targetMaxAcceleration * -1), targetMaxAcceleration);
   float CurrentpercentStepSizeY = setTargetAcceleration_Axis_PercentStepSize(sendTempAcceleratonY, FlightData->yAxisAcceleration, .6 /* Lower is smoother but slower */, .1);   // Acceleration Control // System Fucntions Corrently   :D <-- Happy face!



   // X-axis
   sendTempAcceleratonZ = UKismetMathLibrary::FClamp(TARGET_Z_AXIS_ACCELERATION, (targetMaxAcceleration * -1), targetMaxAcceleration);
   float CurrentpercentStepSizeZ = setTargetAcceleration_Axis_PercentStepSize(sendTempAcceleratonZ, FlightData->zAxisAcceleration, .01 /* Lower is smoother but slower */, .1);   // Acceleration Control // System Fucntions Corrently   :D <-- Happy face!



   //add the total step size then divided its sup between the two, at a certian point, each will have a different step size
   float currentZ_Axis_AverageStepSize = (singleFrontModule->getForwardReverseThrusterStrength() + singleRearModule->getForwardReverseThrusterStrength()) / 2;

   float currentX_Axis_AverageStepSize = (
      frontRightMainThruster->getForwardReverseThrusterStrength() +
      upperRearRightMainThruster->getForwardReverseThrusterStrength() +
      lowerRearRightMainThruster->getForwardReverseThrusterStrength() +
      frontLeftMainThruster->getForwardReverseThrusterStrength() +
      upperRearLeftMainThruster->getForwardReverseThrusterStrength() +
      lowerRearLeftMainThruster->getForwardReverseThrusterStrength()) / 6;



   //float currentY_Axis_AverageStepSize = UKismetMathLibrary::SafeDivide
   //((
   //   frontRightMainThruster->getCurrentPushSideThrusterStrength() +
   //   upperRearRightMainThruster->getCurrentPushSideThrusterStrength() +
   //   lowerRearRightMainThruster->getCurrentPushSideThrusterStrength() +
   //   frontLeftMainThruster->getCurrentPushSideThrusterStrength() +
   //   upperRearLeftMainThruster->getCurrentPushSideThrusterStrength() +
   //   lowerRearLeftMainThruster->getCurrentPushSideThrusterStrength())
   //   ,
   //   3
   //);
      
   float currentY_Axis_AverageStepSize = (frontRightMainThruster->getCurrentPushSideThrusterStrength() + frontLeftMainThruster->getCurrentPushSideThrusterStrength() );
   //currentY_Axis_AverageStepSize = UKismetMathLibrary::FClamp(currentY_Axis_AverageStepSize, -100, 100); // First Derective Clamp thrust output.
   //currentY_Axis_AverageStepSize += CurrentpercentStepSizeY;
   //float alloted_Y_Axis_PercentThrust = currentY_Axis_AverageStepSize;


   tempstate = UKismetMathLibrary::FClamp(tempstate, -100, 100); // First Derective Clamp thrust output.
   tempstate += CurrentpercentStepSizeY;
   //float alloted_Y_Axis_PercentThrust = currentY_Axis_AverageStepSize;

      


   currentZ_Axis_AverageStepSize = UKismetMathLibrary::FClamp(currentZ_Axis_AverageStepSize, -100, 100); // First Derective Clamp thrust output.
   currentZ_Axis_AverageStepSize += CurrentpercentStepSizeZ;
   float alloted_Z_Axis_PercentThrust = currentZ_Axis_AverageStepSize;




   currentX_Axis_AverageStepSize = UKismetMathLibrary::FClamp(currentX_Axis_AverageStepSize, -100, 100); // First Derective Clamp thrust output.
   currentX_Axis_AverageStepSize += CurrentpercentStepSizeX;
   float alloted_X_Axis_PercentThrust = currentX_Axis_AverageStepSize;







   //UE_LOG(LogTemp, Warning, /*TEXT("XR: %f"),*/ TEXT("%f"), CurrentpercentStepSizeY);
   //UE_LOG(LogTemp, Warning, /*TEXT("XR: %f"),*/ TEXT("%f"), tempstate);
   // SIDE THRUSTERS

   sideThrustersSidePercentDistribution(tempstate /* Positive percent means craft moves from left-->Right. */, .01);







   singleFrontModule->setForwardReverseThrusterStrength(alloted_Z_Axis_PercentThrust);
   singleRearModule->setForwardReverseThrusterStrength(alloted_Z_Axis_PercentThrust);





   // Right side of Ship
   frontRightMainThruster->setForwardReverseThrusterStrength(alloted_X_Axis_PercentThrust);
   upperRearRightMainThruster->setForwardReverseThrusterStrength(alloted_X_Axis_PercentThrust);
   lowerRearRightMainThruster->setForwardReverseThrusterStrength(alloted_X_Axis_PercentThrust);


   // Life Side of Ship
   frontLeftMainThruster->setForwardReverseThrusterStrength(alloted_X_Axis_PercentThrust);
   upperRearLeftMainThruster->setForwardReverseThrusterStrength(alloted_X_Axis_PercentThrust);
   lowerRearLeftMainThruster->setForwardReverseThrusterStrength(alloted_X_Axis_PercentThrust);






   //lowerRearRightMainThruster->setForwardReverseThrusterStrength(100);
   //lowerRearLeftMainThruster->setForwardReverseThrusterStrength(100);
   //TODO Pitch Equalizer Between the two thrusters
   //pitchLockStabilization(FlightData->xAxisAngle, 0, allotedThrustPoolToBeUsedtoStabilize, .01);

/***********************************************************************************
* UPDATE DEBUG CODE
***********************************************************************************/


   FlightData->xAxisThrust = alloted_X_Axis_PercentThrust;

   FlightData->yAxisThrust = tempstate;

   FlightData->zAxisThrust = alloted_Z_Axis_PercentThrust;

   FlightData->worldSpaceRotationQuat = UKismetMathLibrary::Conv_RotatorToQuaternion(this->parentMesh->GetRelativeRotation());
// END UPDATE DEBUG CODE
}


float Aflightinterface::getTargetAccelerationBasedOnGoalWorldLocation(float current_Velocity, float Target_Goal, float Current_Loaction, float Asked_Time_To_Stop)
{
   /***********************************************************************************
   * Goal --->
   * Get craft to a target velocity of Zero and to Target stop point
   *
   * USING --->
   *  EQUAATION
   *     {Solve for acceleleration}
   *     d = (Velocity_inital * TIME) + (.5 * Target Acceleration To Stop * (TIME ^2))
   *
   * FUNCTION EQUALS
   *     Target Acceleration To Stop = (Distance - Velocity_inital * TIME) / (.5 * (TIME ^ 2))
   *
   * Accleeration needed to stop will be a goal based function. seeing that the craft
   * will always have a verying weight, the system will have to guess and check its
   *
   ***********************************************************************************/



   float tempOffsetDistance = Target_Goal - Current_Loaction;

   float numerator = tempOffsetDistance - (current_Velocity * Asked_Time_To_Stop);
   float denominator = .5 * UKismetMathLibrary::Square(Asked_Time_To_Stop);

   float tempState = UKismetMathLibrary::SafeDivide(numerator, denominator);

   return tempState;
}



/***********************************************************************************
* STATUS ===== COMPLETE
***********************************************************************************/
/*
*   Will return a positave or negitive step size.
*/
float Aflightinterface::setTargetAcceleration_Axis_PercentStepSize(float target_Acceleration, float current_Acceleration, const float aggression_Curve /* Lower is smoother but slower */, float const allowed_DeadSpace_in_Final_Acceleration)
{
   /***********************************************************************************
   * Goal --->
   *  
   * USING --->
   * EQUATION of
   *     
   ***********************************************************************************/
   
   if (current_Acceleration < (target_Acceleration - allowed_DeadSpace_in_Final_Acceleration))
   {
      // Inverse Trough Function
      
      // this section needs to increase the craft acceleration
      float x = current_Acceleration;
      float tempState = UKismetMathLibrary::Square((x * aggression_Curve)) + 1;
      float percentStepSize = (UKismetMathLibrary::SafeDivide(-1, tempState) * -1) + 1;

      return percentStepSize;
   }
   else if (current_Acceleration > (target_Acceleration + allowed_DeadSpace_in_Final_Acceleration))
   {
      // Normal Trough Function
      
      // this section needs to DECREASE the craft acceleration
      float x = current_Acceleration;
      float tempState = UKismetMathLibrary::Square((x * aggression_Curve)) + 1;
      float percentStepSize = (1 - (UKismetMathLibrary::SafeDivide(-1, tempState))) * -1;

      return percentStepSize;
   }
   else
   {
      // No step size change will be required
      return 0;
   }
}




void Aflightinterface::sideThrustersSidePercentDistribution(float allotted_thrust_percentage /* Positive percent means craft moves from left-->Right. */, float alloted_DeadSpace)
{

   float side_thruster_Percent_To_Which_Side_push = allotted_thrust_percentage; // Positive percent means craft moves from left-->Right.

      // Right side of Ship




   
  // UE_LOG(LogTemp, Warning, /*TEXT("XR: %f"),*/ TEXT("%f"), lowerRearRightMainThruster->getCurrentPushSideThrusterStrength());


   if (side_thruster_Percent_To_Which_Side_push < 0 - alloted_DeadSpace)
   {
      // Push ship from LEFT-->Right
      //UE_LOG(LogTemp, Warning, /*TEXT("XR: %f"),*/ TEXT("To right called"));



      // This needs to swap from negitive to positave values.
      float tempRightTORight = side_thruster_Percent_To_Which_Side_push * (-1);
      // Right side of Ship
      frontRightMainThruster->setPushThrusterStrength(tempRightTORight);
      upperRearRightMainThruster->setPushThrusterStrength(tempRightTORight);
      lowerRearRightMainThruster->setPushThrusterStrength(tempRightTORight);

      frontLeftMainThruster->setPushThrusterStrength(0);
      upperRearLeftMainThruster->setPushThrusterStrength(0);
      lowerRearLeftMainThruster->setPushThrusterStrength(0);
   }
   else if (side_thruster_Percent_To_Which_Side_push > 0 + alloted_DeadSpace)
   {
      // Push ship from RIGHT-->LEFT
      //UE_LOG(LogTemp, Warning, /*TEXT("XR: %f"),*/ TEXT("To Leff called"));
      // Life Side of Ship

      float tempRightTOLeft = side_thruster_Percent_To_Which_Side_push;

      frontLeftMainThruster->setPushThrusterStrength(tempRightTOLeft);
      upperRearLeftMainThruster->setPushThrusterStrength(tempRightTOLeft);
      lowerRearLeftMainThruster->setPushThrusterStrength(tempRightTOLeft);

      frontRightMainThruster->setPushThrusterStrength(0);
      upperRearRightMainThruster->setPushThrusterStrength(0);
      lowerRearRightMainThruster->setPushThrusterStrength(0);
   }

}


/*
* TODO 
*/
// Directional Lock system Moduels
void Aflightinterface::pitchLockStabilization(float current_craft_pitch, float target_craft_pitch, float allotted_thrust_percentage, float alloted_DeadSpace)
{
   float splitPercentThrust = allotted_thrust_percentage;
   float percentOfSet = 0;  // if value is negitive, then there is too much negitive pitch, and vis versa




   if (true)
   {
      float percentAllotedToThisModule = 1;

      //singleFrontModule->setForwardReverseThrusterStrength(splitPercentThrust * .6);
      //singleRearModule->setForwardReverseThrusterStrength(splitPercentThrust * .4);



   }
   else if (false)
   {

   }



}



void Aflightinterface::rollLockStabilization()
{

}
void Aflightinterface::yawLockStabilization()
{

}








/*
* Call every tick
*/
void Aflightinterface::parentMeshAxisValues(USceneComponent* targetMeshToAnalyze)
{

   if (targetMeshToAnalyze && FlightData) // Nullptr check
   {
      UPrimitiveComponent* primitaveMeshRef = Cast<UPrimitiveComponent>(targetMeshToAnalyze);

      FlightData->initalTickVelocity_AcclerationCalulation =  targetMeshToAnalyze->GetComponentVelocity(); // Not used to retreve veloicty of each indivual axis.
      

      FlightData->worldSpaceLocation = targetMeshToAnalyze->GetComponentLocation();

      FlightData->xAxisVelocity = targetMeshToAnalyze->GetComponentVelocity().X;
      FlightData->yAxisVelocity = targetMeshToAnalyze->GetComponentVelocity().Y;
      FlightData->zAxisVelocity = targetMeshToAnalyze->GetComponentVelocity().Z;



      FlightData->xAxisRotationVelocity = primitaveMeshRef->GetPhysicsAngularVelocityInDegrees(FName("none")).X;
      FlightData->yAxisRotationVelocity = primitaveMeshRef->GetPhysicsAngularVelocityInDegrees(FName("none")).Y;
      FlightData->zAxisRotationVelocity = primitaveMeshRef->GetPhysicsAngularVelocityInDegrees(FName("none")).Z;



      FlightData->xAxisAngle = targetMeshToAnalyze->GetComponentRotation().Roll;
      FlightData->yAxisAngle = targetMeshToAnalyze->GetComponentRotation().Pitch;
      FlightData->zAxisAngle = targetMeshToAnalyze->GetComponentRotation().Yaw;


      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      /* Start Calculations for finding each Axis current acceleration */
      FVector XY_Z = FlightData->initalTickVelocity_AcclerationCalulation - FlightData->PreviousTickVelocity_AcclerationCalulation;
      currentWorldDeltaTimeInVectorForm = FVector(UGameplayStatics::GetWorldDeltaSeconds(GetWorld()));

      

      currentAllAxisAccleration  = UKismetMathLibrary::Divide_VectorVector(XY_Z, currentWorldDeltaTimeInVectorForm);


      FlightData->xAxisAcceleration = currentAllAxisAccleration.X;
      FlightData->yAxisAcceleration = currentAllAxisAccleration.Y;
      FlightData->zAxisAcceleration = currentAllAxisAccleration.Z;



      FlightData->PreviousTickVelocity_AcclerationCalulation = FlightData->initalTickVelocity_AcclerationCalulation;
      /* End Calculations for finding each Axis current acceleration */
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



      // Create Line trace Start And End points for calualtions
      FVector STARTLineTraceLocation = targetMeshToAnalyze->GetComponentLocation();
      FVector ENDtLineTraceLocation = UKismetMathLibrary::Subtract_VectorVector(STARTLineTraceLocation, FVector(0,0,7777777777));


      //Convert mesh to AActor type to append to ignor actor list.
      AActor* convertToAACtor = Cast<AActor>(targetMeshToAnalyze);
      const TArray<AActor*> ActorsToIgnore = { convertToAACtor };

      // Create FHitResult struct to be used to find altitude and ground it results.
      FHitResult hitResultFromLineTrace = FHitResult();
      
      const FCollisionQueryParams queryParams = FCollisionQueryParams::DefaultQueryParam;
      const FCollisionResponseParams responseParam = FCollisionResponseParams::DefaultResponseParam;

      // Run Line Trace By Channel
      bool lineTraceSucceed = GetWorld()->LineTraceSingleByChannel
      (
         hitResultFromLineTrace,
         STARTLineTraceLocation,
         ENDtLineTraceLocation,
         ECollisionChannel::ECC_Visibility,
         queryParams,
         responseParam
      );



      // Get mesh altitude to ground (not world Zero)
      //FlightData->altitude = (targetMeshToAnalyze->GetComponentLocation().Z) - (hitResultFromLineTrace->ImpactPoint.Z);



     // DEBUG OUTPUT
     /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     
     // FString output = 
     //    "XR: " + FString::SanitizeFloat(targetMeshFlightData->xAxisRotationVelocity, 5) + 
     //    "YR: " + FString::SanitizeFloat(targetMeshFlightData->yAxisRotationVelocity, 5) +
     //    "ZR: " + FString::SanitizeFloat(targetMeshFlightData->zAxisRotationVelocity, 5);
     //UE_LOG(LogTemp, Warning, /*TEXT("XR: %f"),*/ TEXT("%s"), *output);


     //FString output =
     //   "XR: " + FString::SanitizeFloat(targetMeshFlightData->xAxisAngle, 5) +
     //   "YR: " + FString::SanitizeFloat(targetMeshFlightData->yAxisAngle, 5) +
     //   "ZR: " + FString::SanitizeFloat(targetMeshFlightData->zAxisAngle, 5);
     //UE_LOG(LogTemp, Warning, /*TEXT("XR: %f"),*/ TEXT("%s"), *output);


      FString output =
        "XR: " + FString::SanitizeFloat(FlightData->xAxisAcceleration, 5) +
        "YR: " + FString::SanitizeFloat(FlightData->yAxisAcceleration, 5) +
        "ZR: " + FString::SanitizeFloat(FlightData->zAxisAcceleration, 5);
     //UE_LOG(LogTemp, Warning, /*TEXT("XR: %f"),*/ TEXT("%s"), *output);
     // 
     // 
     // DEBUG OUTPUT
     /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


   }
   else
   {
      FlightData->xAxisVelocity = 0;
      FlightData->yAxisVelocity = 0;
      FlightData->zAxisVelocity = 0;

      FlightData->xAxisRotationVelocity = 0;
      FlightData->xAxisRotationVelocity = 0;
      FlightData->xAxisRotationVelocity = 0;

      FlightData->xAxisAcceleration = 0;
      FlightData->yAxisAcceleration = 0;
      FlightData->zAxisAcceleration = 0;

      FlightData->xAxisAngle = 0;
      FlightData->yAxisAngle = 0;
      FlightData->zAxisAngle = 0;


      FlightData->altitude = 0;
   }

}



void Aflightinterface::runInitRotationControl()
{
   

}






void Aflightinterface::BradcastDelegateToWorld()
{
   if (myNewDelegate.IsBound())
      myNewDelegate.Broadcast();
}





//void Aflightinterface::runEqulizer()
//{
//   float AltDeadZone = 0;
//   int masterup = 60000;
//   int masterPitch = 2000;
//   
//   if (targetMeshFlightData->altitude < desired_Altitude - AltDeadZone)
//   {
//      if (targetMeshFlightData->zAxisVelocity < maxUpVelocity && !thusterAtMaxPower())
//      {
//         thruster_FR->ThrustStrength += masterup;
//         thruster_FL->ThrustStrength += masterup;
//         thruster_RR->ThrustStrength += masterup;
//         thruster_RL->ThrustStrength += masterup;
//      }
//      else if (targetMeshFlightData->zAxisVelocity > maxUpVelocity && !thusterAtMinPower())
//      {
//         if (!(thruster_FR->ThrustStrength < 0) )
//            thruster_FR->ThrustStrength -= masterup;
//         if (!(thruster_FL->ThrustStrength < 0))
//            thruster_FL->ThrustStrength -= masterup;
//         if (!(thruster_RR->ThrustStrength < 0))
//            thruster_RR->ThrustStrength -= masterup;
//         if (!(thruster_RL->ThrustStrength < 0))
//            thruster_RL->ThrustStrength -= masterup;
//      }
//
//
//   }
//   else if (targetMeshFlightData->altitude > desired_Altitude + AltDeadZone)
//   {
//      if (targetMeshFlightData->zAxisVelocity < maxUpVelocity * (-1) && !thusterAtMaxPower())
//      {
//         thruster_FR->ThrustStrength += masterup;
//         thruster_FL->ThrustStrength += masterup;
//         thruster_RR->ThrustStrength += masterup;
//         thruster_RL->ThrustStrength += masterup;
//      }
//      else if (targetMeshFlightData->zAxisVelocity > maxUpVelocity*(-1) && !thusterAtMinPower())
//      {
//         if (!(thruster_FR->ThrustStrength < 0))
//            thruster_FR->ThrustStrength -= masterup;
//         if (!(thruster_FL->ThrustStrength < 0))
//            thruster_FL->ThrustStrength -= masterup;
//         if (!(thruster_RR->ThrustStrength < 0))
//            thruster_RR->ThrustStrength -= masterup;
//         if (!(thruster_RL->ThrustStrength < 0))
//            thruster_RL->ThrustStrength -= masterup;
//      }
//
//   }
//
//
//
//
//   //if (current_Pitch > 0 && current_Pitch < (90 - AltDeadZone))
//   //{
//   //   thruster_FR->ThrustStrength += masterPitch;
//   //   thruster_FL->ThrustStrength += masterPitch;
//
//   //   if (!(thruster_RR->ThrustStrength < 0))
//   //      thruster_RR->ThrustStrength -= masterPitch;
//
//   //   if (!(thruster_RL->ThrustStrength < 0))
//   //   {
//   //      thruster_RL->ThrustStrength -= masterPitch;
//   //      
//   //   }
//
//
//   //   //thruster_FR->Activate(true);
//   //   //thruster_FL->Activate(true);
//   //   //thruster_RR->Deactivate();
//   //   //thruster_RL->Deactivate();
//
//   //}
//   //else if (current_Pitch < 180 && current_Pitch > (90 + AltDeadZone))
//   //{
//   //   if (!(thruster_FR->ThrustStrength < 0))
//   //      thruster_FR->ThrustStrength -= masterPitch;
//   //   if (!(thruster_FL->ThrustStrength < 0))
//   //      thruster_FL->ThrustStrength -= masterPitch;
//
//
//   //   thruster_RR->ThrustStrength += masterPitch;
//   //   thruster_RL->ThrustStrength += masterPitch;
//   //   //thruster_FR->Deactivate();
//   //   //thruster_FL->Deactivate();
//   //   //thruster_RR->Activate(true);
//   //   //thruster_RL->Activate(true);
//   //}
//
//
//
//
//
//
//
//
//
//   //if (current_Roll > 0 && current_Roll < 90 - AltDeadZone)
//   //{
//   //  // thruster_FR->ThrustStrength += masterPitch;
//   //   //thruster_FL->ThrustStrength -= masterPitch;
//   //   if (!(thruster_FL->ThrustStrength < 0))
//   //      thruster_FL->ThrustStrength -= masterPitch;
//
//   //  // thruster_RR->ThrustStrength += masterPitch;
//
//   //   if (!(thruster_RL->ThrustStrength < 0))
//   //      thruster_RL->ThrustStrength -= masterPitch;
//   //   //thruster_RL->ThrustStrength -= masterPitch;
//
//
//   //   //thruster_FR->Activate(true);
//   //   //thruster_FL->Deactivate();
//   //   //thruster_RR->Activate(true);
//   //   //thruster_RL->Deactivate();
//   //}
//   //else if (current_Roll < 180 && current_Roll > 90 + AltDeadZone)
//   //{
//   //   //thruster_FR->ThrustStrength -= masterPitch;
//
//   //   if (!(thruster_FR->ThrustStrength < 0))
//   //      thruster_FR->ThrustStrength -= masterPitch;
//   //  // thruster_FL->ThrustStrength += masterPitch;
//
//
//   //   if (!(thruster_RR->ThrustStrength < 0))
//   //      thruster_RR->ThrustStrength -= masterPitch;
//   //   //thruster_RR->ThrustStrength -= masterPitch;
//   // //  thruster_RL->ThrustStrength += masterPitch;
//
//   //   //thruster_FR->Deactivate();
//   //   //thruster_FL->Activate(true);
//   //   //thruster_RR->Deactivate();
//   //   //thruster_RL->Activate(true);
//   //}
//
//   //UE_LOG(LogTemp, Warning, TEXT("Output: %f"), current_Roll);
//   //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));
//
//}




void Aflightinterface::bindRequiredComponents(
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

   USceneComponent* targetMeshComponent)
{

   this->parentMesh = targetMeshComponent;

   FVector location = this->GetOwner()->GetActorLocation(); // "This" is consider the owner location
   FRotator rotation = this->GetOwner()->GetActorRotation();// "This" is consider the owner Rotation


   FActorSpawnParameters SpawnInfo;
   SpawnInfo.Owner = this;
   


   // FRONT ENGINE MODULE BINDING

   if (frontRightEngineModule)
   { 
      UChildActorComponent* temms = Cast<UChildActorComponent>(frontRightEngineModule);
      frontRightEngineActor = temms->GetChildActor();
      FName tempname = "";
      frontRightEngineActor->K2_AttachToComponent(targetMeshComponent, tempname, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

      
      if (FRM && FRMrev && FRU && FRUrev && FRMPR)
      {
         frontRightMainThruster = GetWorld()->SpawnActor<AthrusterLinker>(location, rotation, SpawnInfo);
         frontRightMainThruster->BindPhysicsThruster(frontRightEngineActor, FRM, FRMrev, FRU, FRUrev, FRMPR, false, AthrusterLinker::Right, AthrusterLinker::IndependentThrusters);
      
         // Create Default startup values.
         frontRightMainThruster->setThrusterPercentPitch(TargetSession->frontRightEnginePercentPitch);
      }
   }

   if (frontLeftEngineModule)
   {
      UChildActorComponent* temms = Cast<UChildActorComponent>(frontLeftEngineModule);
      frontLeftEngineActor = temms->GetChildActor();
      FName tempname = "";
      frontLeftEngineActor->K2_AttachToComponent(targetMeshComponent, tempname, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

   if (FLM && FLMrev && FLU && FLUrev && FLMPR)
      {
      frontLeftMainThruster = GetWorld()->SpawnActor<AthrusterLinker>(location, rotation, SpawnInfo);
      frontLeftMainThruster->BindPhysicsThruster(frontLeftEngineActor,FLM, FLMrev, FLU, FLUrev, FLMPR, false, AthrusterLinker::Left, AthrusterLinker::IndependentThrusters);
      
      // Create Default startup values.
      frontLeftMainThruster->setThrusterPercentPitch(TargetSession->frontLeftEnginePercentPitch);
      }
   }



   // UPPER REAR ENGINE MODULE BINDING

   if (upperRearRightEngineModule)
   {
      UChildActorComponent* temms = Cast<UChildActorComponent>(upperRearRightEngineModule);
      upperRearRightEngineActor = temms->GetChildActor();
      FName tempname = "";
      upperRearRightEngineActor->K2_AttachToComponent(targetMeshComponent, tempname, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
      if (URRM && URRMrev && URRU && URRUrev && URRMPR)
      {
         upperRearRightMainThruster = GetWorld()->SpawnActor<AthrusterLinker>(location, rotation, SpawnInfo);
         upperRearRightMainThruster->BindPhysicsThruster(upperRearRightEngineActor,URRM, URRMrev, URRU, URRUrev, URRMPR, false, AthrusterLinker::Right, AthrusterLinker::IndependentThrusters);
      
         // Create Default startup values.
         upperRearRightMainThruster->setThrusterPercentPitch(TargetSession->rearUpperRightEnginePercentPitch);
      }
   }

   if (upperRearLeftEngineModule)
   {
      UChildActorComponent* temms = Cast<UChildActorComponent>(upperRearLeftEngineModule);
      upperRearLeftEngineActor = temms->GetChildActor();
      FName tempname = "";
      upperRearLeftEngineActor->K2_AttachToComponent(targetMeshComponent, tempname, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
      if (URLM && URLMrev && URLU && URLUrev && URLMPR)
      {
         upperRearLeftMainThruster = GetWorld()->SpawnActor<AthrusterLinker>(location, rotation, SpawnInfo);
         upperRearLeftMainThruster->BindPhysicsThruster(upperRearLeftEngineActor,URLM, URLMrev, URLU, URLUrev, URLMPR, false, AthrusterLinker::Left, AthrusterLinker::IndependentThrusters);
      
      
         // Create Default startup values.
         upperRearLeftMainThruster->setThrusterPercentPitch(TargetSession->rearUpperLeftEnginePercentPitch);
      }
   }


   // LOWER REAR ENGINE MODULE BINDING

   if (lowerRearRightEngineModule)
   {
      UChildActorComponent* temms = Cast<UChildActorComponent>(lowerRearRightEngineModule);
      lowerRearRightEngineActor = temms->GetChildActor();
      FName tempname = "";
      lowerRearRightEngineActor->K2_AttachToComponent(targetMeshComponent, tempname, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
      if (LRRM && LRRMrev && LRRU && LRRUrev && LRRMPR)
      {
         lowerRearRightMainThruster = GetWorld()->SpawnActor<AthrusterLinker>(location, rotation, SpawnInfo);
         lowerRearRightMainThruster->BindPhysicsThruster(lowerRearRightEngineActor, LRRM, LRRMrev, LRRU, LRRUrev, LRRMPR, false, AthrusterLinker::Right, AthrusterLinker::IndependentThrusters);
      
         // Create Default startup values.
         lowerRearRightMainThruster->setThrusterPercentPitch(TargetSession->RearLowerRightEnginePercentPitch);
      }
   }

   if (lowerRearLeftEngineModule)
   {
      UChildActorComponent* temms = Cast<UChildActorComponent>(lowerRearLeftEngineModule);
      lowerRearLeftEngineActor = temms->GetChildActor();
      FName tempname = "";
      lowerRearLeftEngineActor->K2_AttachToComponent(targetMeshComponent, tempname, EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
      if (LRLM && LRLMrev && LRLU && LRLUrev && LRLMPR)
      {
         lowerRearLeftMainThruster = GetWorld()->SpawnActor<AthrusterLinker>(location, rotation, SpawnInfo);
         lowerRearLeftMainThruster->BindPhysicsThruster(lowerRearLeftEngineActor, LRLM, LRLMrev, LRLU, LRLUrev, LRLMPR, false, AthrusterLinker::Left, AthrusterLinker::IndependentThrusters);
      
         // Create Default startup values.
         lowerRearLeftMainThruster->setThrusterPercentPitch(TargetSession->rearLowerLeftEnginePercentPitch);
      }
   }




   if (FFR && FFL && FRL && FRR)
   {
      singleFrontModule = GetWorld()->SpawnActor<AthrusterLinker>(location, rotation, SpawnInfo);
      singleFrontModule->BindPhysicsThruster(nullptr, FFR, FFL, FRL, FRR, nullptr, false, AthrusterLinker::Center, AthrusterLinker::SingleUnit);


   }

   if (RFR && RFL && RRL && RRR)
   {
      singleRearModule = GetWorld()->SpawnActor<AthrusterLinker>(location, rotation, SpawnInfo);
      singleRearModule->BindPhysicsThruster(nullptr, RFR, RFL, RRL, RRR, nullptr, false, AthrusterLinker::Center, AthrusterLinker::SingleUnit);

   }





      NNO = GetWorld()->SpawnActor<AneuralNetworkOptimization>(location, rotation, SpawnInfo);



}



bool Aflightinterface::InitThrusterTest()
{
   int masterup = 0;
   if (true)
   {

      //thrusterLinker_FR->getPointerToThruster()->Activate(true);
      //thrusterLinker_FL->getPointerToThruster()->Activate(true);
      //thrusterLinker_RR->getPointerToThruster()->Activate(true);
      //thrusterLinker_RL->getPointerToThruster()->Activate(true);

      //thruster_FR->Activate(true);
      //thruster_FL->Activate(true);
      //thruster_RR->Activate(true);
      //thruster_RL->Activate(true);
      //

      //thrusterLinker_FR->getPointerToThruster()->ThrustStrength = masterup;
      //thrusterLinker_FL->getPointerToThruster()->ThrustStrength = masterup;
      //thrusterLinker_RR->getPointerToThruster()->ThrustStrength = masterup;
      //thrusterLinker_RL->getPointerToThruster()->ThrustStrength = masterup;

      return true;
   }
   else
   {
      return false;
   }

}



void Aflightinterface::BindDelegate()
{
   myNewDelegate.AddUniqueDynamic(this, &Aflightinterface::SystemSyncedTick);
}