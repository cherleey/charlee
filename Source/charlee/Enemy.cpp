// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "MeleeWeapon.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AttackAnimTime = 0;
	Speed = 20;
	Hp = 100;
	Damage = 1;
	AttackTimeout = 1.5f;
	AttackAnimTimeout = 1.5f;
	eState = IDLE;
	Target = NULL;
	bAttacking = false;

	SightSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SightSphere"));
	SightSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnSightOverlapBegin);
	SightSphere->AttachTo(RootComponent);

	AttackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRangeSphere"));
	AttackRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnAttackRangeOverlapBegin);
	AttackRangeSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnAttackRangeOverlapEnd);
	AttackRangeSphere->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (eState)
	{
	case IDLE:
		break;
		
	case RUN:
		MoveTo(Target, DeltaTime);
		break;

	case ATTACK:
		AttackAnimTime += DeltaTime;

		if (!bAttacking)
		{
			eState = RUN;
			AttackAnimTime = 0;
			break;
		}
		
		break;

	case HIT:
		break;

	case DIE:
		break;
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::MoveTo(AActor * OtherActor, float DeltaTime)
{
	if (!Target)
		return;

	FVector Dir = Target->GetActorLocation() - GetActorLocation();
	Dir.Normalize();
	FRotator Rotation = Dir.Rotation();
	Rotation.Pitch = 0;

	AddMovementInput(Dir, Speed * DeltaTime);
	RootComponent->SetWorldRotation(Rotation);
}

void AEnemy::OnSightOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (bAttacking)
		return;

	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && eState != RUN)
	{
		eState = RUN;
		Target = OtherActor;
	}
}

void AEnemy::OnAttackRangeOverlapBegin_Implementation(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && eState != ATTACK)
	{
		eState = ATTACK;
		Target = OtherActor;
		bAttacking = true;
	}
}

void AEnemy::OnAttackRangeOverlapEnd_Implementation(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		if (eState == ATTACK && AttackAnimTime <= AttackAnimTimeout)
		{
			bAttacking = true;
			return;
		}
		else if (eState == ATTACK)
		{
			AttackAnimTime = 0;
			bAttacking = false;
		}
	}
}

bool AEnemy::IsAttacking()
{
	if (eState == ATTACK)
		return true;
	else
		return false;
}

void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (BPMeleeWeapon)
	{
		MeleeWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(BPMeleeWeapon);

		if (BPMeleeWeapon)
		{
			const USkeletalMeshSocket* socket = this->GetMesh()->GetSocketByName("RightHandSocket");
			socket->AttachActor((AActor*)MeleeWeapon, this->GetMesh());
		}
	}
}
