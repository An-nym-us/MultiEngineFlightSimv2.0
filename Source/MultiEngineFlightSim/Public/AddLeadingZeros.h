// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AddLeadingZeros.generated.h"

/**
 * 
 */
UCLASS()
class MULTIENGINEFLIGHTSIM_API UAddLeadingZeros : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
      UFUNCTION(BlueprintCallable)
      static FString AddLeadingZeroes(const int32& UglyNumber, const int Length);

};
