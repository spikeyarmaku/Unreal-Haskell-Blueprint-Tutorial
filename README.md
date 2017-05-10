# Using Haskell + Stack with Unreal Engine 4 on Windows (Blueprint version)

1. Create a project in UnrealEngine. I'm going to name it "HsDllTest"    
![New project](https://github.com/gizmo-mk0/Unreal-Haskell/raw/master/_images/new_project.png)
2. Go to the project folder, and create a folder. I will name it "Plugins".    
![New directory](https://github.com/gizmo-mk0/Unreal-Haskell/raw/master/_images/folder.PNG)
3. Create a new stack project in it. I will call it "HsLib" and use the "simple-library" template:    
```...\HsDllTest\Plugins> stack new HsLib simple-library```
4. In the .cabal file, add this line: "ghc-options: -shared"    
![New entry in the cabal file](https://github.com/gizmo-mk0/Unreal-Haskell/raw/master/_images/cabal.png)
5. Paste this to Lib.hs:
```haskell
{-# LANGUAGE ForeignFunctionInterface #-}

module Lib
    ( hsAdder
    ) where

import Foreign.C.Types

foreign export ccall hsAdder :: CInt -> CInt -> CInt

hsAdder :: CInt -> CInt -> CInt
hsAdder a b = a + b
```
6. Use the `stack build` command to build the .dll file. (You'll find the "Lib_stub.h" file in ".stack-work/dist/????????/build", if you need it for something. Also note, that running `stack build` also creates a HsDlltest.dll.a file. It is a static library, but isn't necessary for our purposes.)
7. Add a new C++ library to the Unreal Project. Choose "Blueprint Function Library". I will name it HsDllTestBFL
8. Paste it into the "HsDllTestBFL.cpp" file:
```cpp
// Fill out your copyright notice in the Description page of Project Settings.

#include "HsDllTest.h"
#include "HsDllTestBFL.h"

typedef int32(*_hsAdder)(int32 a, int32 b); // Declare a method to store the DLL method hsAdder.
typedef void(*_hs_init)(int* argc, char** argv);
typedef void(*_hs_exit)();

_hsAdder m_hsAdder;
_hs_init m_hs_init;
_hs_exit m_hs_exit;

void *v_dllHandle;

#pragma region Load DLL
// Method to import a DLL.
bool UHsDllTestBFL::importDLL(FString folder, FString name)
{
	FString filePath = *FPaths::GamePluginsDir() + folder + "/" + name;

	if (FPaths::FileExists(filePath))
	{
		v_dllHandle = FPlatformProcess::GetDllHandle(*filePath); // Retrieve the DLL.
		if (v_dllHandle != NULL)
		{
			return true;
		}
	}
	return false;	// Return an error.
}
#pragma endregion Load DLL

#pragma region Import Methods

// Imports the method getInvertedBool from the DLL.
bool UHsDllTestBFL::importMethod_hsAdder()
{
	if (v_dllHandle != NULL)
	{
		m_hsAdder = NULL;
		FString procName = "hsAdder";	// Needs to be the exact name of the DLL method.
		m_hsAdder = (_hsAdder)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_hsAdder != NULL)
		{
			return true;
		}
	}
	return false;	// Return an error.
}

bool UHsDllTestBFL::importMethod_hs_init()
{
	if (v_dllHandle != NULL)
	{
		m_hs_init = NULL;
		FString procName = "hs_init";	// Needs to be the exact name of the DLL method.
		m_hs_init = (_hs_init)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_hs_init != NULL)
		{
			return true;
		}
	}
	return false;	// Return an error.
}

bool UHsDllTestBFL::importMethod_hs_exit()
{
	if (v_dllHandle != NULL)
	{
		m_hs_exit = NULL;
		FString procName = "hs_exit";	// Needs to be the exact name of the DLL method.
		m_hs_exit = (_hs_exit)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_hs_exit != NULL)
		{
			return true;
		}
	}
	return false;	// Return an error.
}

#pragma endregion Import Methods

#pragma region Method Calls

// Calls the method getInvertedBoolFromDll that was imported from the DLL.
int32 UHsDllTestBFL::hsAdder(int32 a, int32 b)
{
	if (m_hsAdder != NULL)
	{
		int32 out = int32(m_hsAdder(a, b)); // Call the DLL method with arguments corresponding to the exact signature and return type of the method.
		return out;
	}
	return -1;	// Return an error.
}

void UHsDllTestBFL::hs_init()
{
	if (m_hs_init != NULL)
	{
		m_hs_init(NULL, NULL); // Call the DLL method with arguments corresponding to the exact signature and return type of the method.
	}
}

void UHsDllTestBFL::hs_exit()
{
	if (m_hs_exit != NULL)
	{
		m_hs_exit(); // Call the DLL method with arguments corresponding to the exact signature and return type of the method.
	}
}

#pragma endregion Method Calls


#pragma region Unload DLL
// If you love something  set it free.
void UHsDllTestBFL::freeDLL()
{
	if (v_dllHandle != NULL)
	{
		m_hsAdder = NULL;
		m_hs_init = NULL;
		m_hs_exit = NULL;

		FPlatformProcess::FreeDllHandle(v_dllHandle);
		v_dllHandle = NULL;
	}
}
#pragma endregion Unload DLL
```
9. Paste it into the "HsDllTestBFL.h" file:
```cpp
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

```
10. Hit the compile button in the editor
11. Create a blueprint class. I'll name it "HsDllActor"
12. Create this blueprint:    
![Blueprint](https://github.com/gizmo-mk0/Unreal-Haskell/raw/master/_images/blueprint.png)
13. Compile, and save. Drag it to the map, and press play. Relieve that it works.    
![It works!](https://github.com/gizmo-mk0/Unreal-Haskell/raw/master/_images/it_works.png)
