// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "HsDllTestBFL.generated.h"

/**
 * 
 */
UCLASS()
class HSDLLTEST_API UHsDllTestBFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	public:

	UFUNCTION(BlueprintCallable, Category = "My DLL Library")
	static bool importDLL(FString folder, FString name);
	
	UFUNCTION(BlueprintCallable, Category = "My DLL Library")
	static bool importMethod_hs_init();

	UFUNCTION(BlueprintCallable, Category = "My DLL Library")
	static bool importMethod_hs_exit();

	UFUNCTION(BlueprintCallable, Category = "My DLL Library")
	static bool importMethod_hsAdder();

	UFUNCTION(BlueprintCallable, Category = "My DLL Library")
	static int32 hsAdder(int32 a, int32 b);

	UFUNCTION(BlueprintCallable, Category = "My DLL Library")
	static void hs_init(); // Just give Null, Null as parameters. We call it as a lib, without using command line arguments

	UFUNCTION(BlueprintCallable, Category = "My DLL Library")
	static void hs_exit();

	UFUNCTION(BlueprintCallable, Category = "My DLL Library")
	static void freeDLL();
};
