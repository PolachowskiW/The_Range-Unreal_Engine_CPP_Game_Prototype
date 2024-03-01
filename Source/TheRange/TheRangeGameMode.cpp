// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheRangeGameMode.h"
#include "TheRangeHUD.h"
#include "TheRangeCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "PreviousTime.h"

ATheRangeGameMode::ATheRangeGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATheRangeHUD::StaticClass();

	Targets = 0;
	DestroyedTargets = 0;

}


void ATheRangeGameMode::BeginPlay()
{
	areTargetsDestroyed = false;

	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(Counter, this, &ATheRangeGameMode::ContUpTimer, 1.0f, true, 1.0f);

	DestroyedTargets = 0;

}

void ATheRangeGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TargetDestroyedConfirm();
}

void ATheRangeGameMode::AddTarget()
{
	Targets++;
}

void ATheRangeGameMode::DestroyTarget()
{
	DestroyedTargets++;
}

void ATheRangeGameMode::TargetDestroyedConfirm()
{
	if (DestroyedTargets == Targets)
	{
		areTargetsDestroyed = true;
	}
}

void ATheRangeGameMode::RestartGameplay(bool Won)
{
	if (Won)
	{
		ResetLevel();
	}
	else
	{
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ATheRangeGameMode::ResetLevel, 2.0f, false);
	}
}

void ATheRangeGameMode::ResetLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Range");
}


void ATheRangeGameMode::ContUpTimer()
{
	TimerCount++;
}