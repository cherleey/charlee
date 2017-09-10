// Fill out your copyright notice in the Description page of Project Settings.

#include "Avatar.h"


// Sets default values
AAvatar::AAvatar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAvatar::BeginPlay()
{
	Super::BeginPlay();
	
	MouseSpeed = 200;
	MaxHp = 100;
	Hp = 100;
}

// Called every frame
void AAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Vertical", this, &AAvatar::MoveVertical);
	PlayerInputComponent->BindAxis("Horizontal", this, &AAvatar::MoveHorizontal);
	PlayerInputComponent->BindAxis("Yaw", this, &AAvatar::Yaw);
	PlayerInputComponent->BindAxis("Pitch", this, &AAvatar::Pitch);
}

void AAvatar::MoveVertical(float amount)
{
	if (Controller && amount)
	{
		FVector vertical = GetActorForwardVector();
		AddMovementInput(vertical, amount);
	}
}

void AAvatar::MoveHorizontal(float amount)
{
	if (Controller && amount)
	{
		FVector horizontal = GetActorRightVector();
		AddMovementInput(horizontal, amount);
	}
}

void AAvatar::Yaw(float amount)
{
	AddControllerYawInput(MouseSpeed * amount * GetWorld()->GetDeltaSeconds());
}

void AAvatar::Pitch(float amount)
{
	AddControllerPitchInput(MouseSpeed * amount * GetWorld()->GetDeltaSeconds());
}

float AAvatar::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(ActualDamage >= 0 && Hp >= 0)
		Hp -= ActualDamage;

	if (Hp < 0)
		Hp = 0;

	return ActualDamage;
}