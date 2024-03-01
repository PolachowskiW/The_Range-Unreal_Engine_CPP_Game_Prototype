// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TheRangeGameMode.generated.h"

UCLASS(minimalapi)
class ATheRangeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATheRangeGameMode();

	void RestartGameplay(bool Won);

private:

	void ResetLevel();

public:

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
		int TimerCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int Targets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		int DestroyedTargets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool areTargetsDestroyed;

private:

	FTimerHandle Counter = FTimerHandle();

	void ContUpTimer();

public:

	void BeginPlay() override;

	void AddTarget();

	void DestroyTarget();

	void TargetDestroyedConfirm();

};



