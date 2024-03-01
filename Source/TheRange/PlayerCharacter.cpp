// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
 
#include "Projectile.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"

#include "RangeGameMode.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(40.0f, 95.0f);

	TurnRate = 45.0f;
	LookUpRate = 45.0f;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->AddRelativeLocation(FVector(-39.65f, 1.75f, 64.0f));
	FirstPersonCamera->bUsePawnControlRotation = true;

	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));

	HandsMesh->SetOnlyOwnerSee(true);
	HandsMesh->SetupAttachment(FirstPersonCamera);
	HandsMesh->bCastDynamicShadow = false;
	HandsMesh->CastShadow = false;
	HandsMesh->AddRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	HandsMesh->AddRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	GunMesh->SetOnlyOwnerSee(true);
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle Location"));
	MuzzleLocation->SetupAttachment(GunMesh);
	MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	isSprinting = false;
	isZoomedIn = false;

	isAutomatic = true;
	isSingle = false;
	isReloading = false;

	ReloadTime = 2.f;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GunMesh->AttachToComponent(HandsMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GripPoint"));

	World = GetWorld();

	AnimInstance = HandsMesh->GetAnimInstance();

	LoadedAmmo = 30;
	MaxAmmo = 30;
	CarriedAmmo = 60;


}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::StopFire);

	PlayerInputComponent->BindAction("SwitchFire", IE_Pressed, this, &APlayerCharacter::SwitchFire);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCharacter::StartReload);

	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &APlayerCharacter::ZoomIn);
	PlayerInputComponent->BindAction("ZoomIn", IE_Released, this, &APlayerCharacter::StopZoom);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::StopSprinting);

	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookAtRate);



}


void APlayerCharacter::StartFire()
{
	if (isReloading == false)
	{
		if (isAutomatic && LoadedAmmo > 0)
		{
			FireShot();

			GetWorldTimerManager().SetTimer(TimerHandle_HandleRefire, this, &APlayerCharacter::FireShot, TimeBetweenShots, true);
		}
		else if (isSingle && LoadedAmmo > 0)
		{
			FireShot();
		}
		else
		{
			StopFire();
		}
	}
}

void APlayerCharacter::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_HandleRefire);
}

void APlayerCharacter::FireShot()
{
		if (LoadedAmmo > 0)
		{

			if (World != NULL)
			{
				SpawnRotation = GetControlRotation();

				SpawnLocation = ((MuzzleLocation != nullptr) ?
					MuzzleLocation->GetComponentLocation() :
					GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride =
					ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				World->SpawnActor<AProjectile>(Projectile, SpawnLocation, SpawnRotation, ActorSpawnParams);\
			}


			if (FireSound != NULL)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			if (FireAnimation != NULL && AnimInstance != NULL)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.0f);
			}
			LoadedAmmo = LoadedAmmo - 1;
		}
		else if (LoadedAmmo == 0)
		{
			StopFire();
		}
}

void APlayerCharacter::SwitchFire()
{
	if (isAutomatic)
	{
		isAutomatic = false;
		isSingle = true;
	}
	else if (isSingle)
	{
		isSingle = false;
		isAutomatic = true;
	}
}

void APlayerCharacter::Reload()
{	
	if (LoadedAmmo < MaxAmmo)
	{
		if (CarriedAmmo > 0)
		{
			if (CarriedAmmo >= MaxAmmo)
			{
				CarriedAmmo = CarriedAmmo - MaxAmmo;
				LoadedAmmo = MaxAmmo;
			}
			else if (CarriedAmmo < MaxAmmo)
			{
				LoadedAmmo = CarriedAmmo;
				CarriedAmmo = 0;
			}
			
		}
	}
}

void APlayerCharacter::StopReload()
{
	isReloading = false;
	GetWorldTimerManager().ClearTimer(ReloadDelay);
}


void APlayerCharacter::StartReload()
{
	if (LoadedAmmo <= MaxAmmo && CarriedAmmo > 0)
	{
		isReloading = true;
		UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, GetActorLocation());
		Reload();
		GetWorldTimerManager().SetTimer(ReloadDelay, this, &APlayerCharacter::StopReload, ReloadTime, false);
	}
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * TurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookAtRate(float Rate)
{
	AddControllerPitchInput(Rate * LookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 1500.0f;
	isSprinting = true;
}

void APlayerCharacter::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	isSprinting = false;
}

void APlayerCharacter::ZoomIn()
{
	FirstPersonCamera->SetFieldOfView(50.0f);
	isZoomedIn = true;
}

void APlayerCharacter::StopZoom()
{
	FirstPersonCamera->SetFieldOfView(90.0f);
	isZoomedIn = false;
}


void APlayerCharacter::AddAmmo()
{
	CarriedAmmo += 30;
}


