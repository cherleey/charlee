// Fill out your copyright notice in the Description page of Project Settings.

#include "Avatar.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


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
	
	MouseSpeed = 200.f;
	MaxHp = 100.f;
	Hp = 100.f;
	Damage = 5.f;
	AttackTimeout = 0.2f;
	AttackTime = AttackTimeout;
	bTraced = false;
	bMouseLeftPressed = false;
	PitchAmount = 0;
	APlayerController* PC = Cast<APlayerController>(Controller);
	PC->PlayerCameraManager->ViewPitchMin = -40.f;
	PC->PlayerCameraManager->ViewPitchMax = 60.f;
	OriginLocation = this->GetActorLocation();
}

// Called every frame
void AAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Hp <= 0)
		Respawn();
	
	if(bMouseLeftPressed)
	{
		if (!RayCast())
			return;

		AttackTime += DeltaTime;

		if (AttackTime >= AttackTimeout)
		{
			AGunEffect* Effect = GetWorld()->SpawnActor<AGunEffect>(BPGunEffect, ImpactPoint, FRotator(0));
			Target->TakeDamage(Damage, FDamageEvent(), this->GetInstigatorController(), this);
			AttackTime = 0;
		}
	}
}

// Called to bind functionality to input
void AAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Vertical", this, &AAvatar::MoveVertical);
	PlayerInputComponent->BindAxis("Horizontal", this, &AAvatar::MoveHorizontal);
	PlayerInputComponent->BindAxis("Yaw", this, &AAvatar::Yaw);
	PlayerInputComponent->BindAxis("Pitch", this, &AAvatar::Pitch);
	PlayerInputComponent->BindAction("MouseLeftClick", IE_Pressed, this, &AAvatar::MouseLeftClickPressed);
	PlayerInputComponent->BindAction("MouseLeftClick", IE_Released, this, &AAvatar::MouseLeftClickReleased);
}

void AAvatar::MoveVertical(float amount)
{
	if (Controller && amount)
	{
		FVector vertical = GetActorForwardVector();
		AddMovementInput(vertical, amount * 0.7f);
	}
}

void AAvatar::MoveHorizontal(float amount)
{
	if (Controller && amount)
	{
		FVector horizontal = GetActorRightVector();
		AddMovementInput(horizontal, amount * 0.7f);
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

void AAvatar::MouseLeftClickPressed()
{
	bMouseLeftPressed = true;
}

void AAvatar::MouseLeftClickReleased()
{
	bTraced = false;
	bMouseLeftPressed = false;
	AttackTime = AttackTimeout;
}

bool AAvatar::RayCast()
{
	APlayerController* PC = Cast<APlayerController>(Controller);
	
	if (PC == NULL)
		return false;

	APlayerController* PC = Cast<APlayerController>(Controller);
	PC->ClientPlayCameraShake(BPCameraShake->GetClass(), 20.f);

	FVector CameraLocation;
	FRotator CameraRotaion;
	FHitResult RV_Hit(ForceInit);
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);

	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = true;
	RV_TraceParams.AddIgnoredActor(this);
	
	PC->PlayerCameraManager->GetCameraViewPoint(CameraLocation, CameraRotaion);
	
	FVector Start = CameraLocation + (CameraRotaion.Vector() * 100);
	FVector End = CameraLocation + (CameraRotaion.Vector() * 10000);

	bool bTraced = GetWorld()->LineTraceSingleByChannel(RV_Hit, Start, End, ECC_Pawn, RV_TraceParams);
	
	if (bTraced)
	{
		AttackTime += GetWorld()->DeltaTimeSeconds;

		ImpactPoint = RV_Hit.ImpactPoint;
		Target = RV_Hit.GetActor();		
	}

	return bTraced;
}

void AAvatar::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (BPRangeWeapon)
	{
		RangeWeapon = GetWorld()->SpawnActor<ARangeWeapon>(BPRangeWeapon);

		if (BPRangeWeapon)
		{
			const USkeletalMeshSocket* socket = this->GetMesh()->GetSocketByName("hand_r_socket");
			socket->AttachActor((AActor*)RangeWeapon, this->GetMesh());
		}
	}
}

void AAvatar::Respawn()
{
	Hp = 100.f;
	AttackTime = AttackTimeout;
	bTraced = false;
	bMouseLeftPressed = false;
	PitchAmount = 0;
	this->SetActorLocation(OriginLocation);
}