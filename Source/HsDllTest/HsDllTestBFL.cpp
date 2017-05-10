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
