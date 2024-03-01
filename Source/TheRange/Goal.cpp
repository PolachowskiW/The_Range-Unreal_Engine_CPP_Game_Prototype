// Fill out your copyright notice in the Description page of Project Settings.


#include "Goal.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "TheRangeGameMode.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGoal::AGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GoalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Goal Mesh"));
	RootComponent = GoalMesh;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Component"));
	CollisionComponent->SetupAttachment(GoalMesh);

}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AGoal::OnHit);
	
}

// Called every frame
void AGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoal::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{

	APlayerCharacter* Char = Cast<APlayerCharacter>(OtherActor);

	if (Char)
	{
		ATheRangeGameMode* GameMode = Cast<ATheRangeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (GameMode)
		{
			if (GameMode->DestroyedTargets == GameMode->Targets)
			{
				GameMode->RestartGameplay(true);
			}

		}

	}

}
