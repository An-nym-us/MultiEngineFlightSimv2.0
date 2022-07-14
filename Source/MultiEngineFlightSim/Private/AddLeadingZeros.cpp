// Fill out your copyright notice in the Description page of Project Settings.


#include "AddLeadingZeros.h"

FString UAddLeadingZeros::AddLeadingZeros(const int32& UglyNumber, const int Length)
{
   char Buffer[100];
   int RespCode;
   RespCode = snprintf(Buffer, 100, "%0*d", Length, UglyNumber);
   return FString(ANSI_TO_TCHAR(Buffer));
}