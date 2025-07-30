// Copyright Epic Games, Inc. All Rights Reserved.


#include "FPSurvivalPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "FPSurvivalCameraManager.h"

AFPSurvivalPlayerController::AFPSurvivalPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AFPSurvivalCameraManager::StaticClass();
}

void AFPSurvivalPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}
