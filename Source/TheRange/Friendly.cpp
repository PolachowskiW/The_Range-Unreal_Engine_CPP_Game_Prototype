// Fill out your copyright notice in the Description page of Project Settings.


#include "Friendly.h"
#include "TheRangeGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFriendly::AFriendly()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 1.0f;

}

// Called when the game starts or when spawned
void AFriendly::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFriendly::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFriendly::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFriendly::DealDamage(float DamageAmount)
{
	Health -= DamageAmount;

	if (Health <= 0.0f)
	{
		ATheRangeGameMode* MyGameMode = Cast<ATheRangeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (MyGameMode)
		{
			MyGameMode->RestartGameplay(false);
		}
		
		Destroy();
	}
}

