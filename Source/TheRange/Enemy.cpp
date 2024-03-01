// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "TheRangeGameMode.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 2.0f;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	ATheRangeGameMode* GameMode = Cast<ATheRangeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->AddTarget();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::DealDamage(float DamageAmount)
{
	Health -= DamageAmount;

	if (Health <= 0.0f)
	{
		ATheRangeGameMode* GameMode = Cast<ATheRangeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		GameMode->DestroyTarget();
		Destroy();
	}
}

