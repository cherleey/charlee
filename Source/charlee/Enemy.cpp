// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "MeleeWeapon.h"
#include "RespawnPoint.h"
#include "EngineUtils.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	AttackAnimTime = 1.5f;
	Speed = 20;
	Hp = 100;
	Damage = 1;
	AttackTimeout = 0;
	AttackAnimTimeout = 1.5f;
	eState = IDLE;
	Target = NULL;
	bAttacking = false;
	bInSight = false;
	bInAttackRange = false;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SwitchState();

	switch (eState)
	{
	case IDLE:
		bAttacking = false;
		break;
		
	case RUN:
		if (AttackAnimTime < AttackAnimTimeout && bAttacking)
		{
			AttackAnimTime += DeltaTime;
			break;
		}
		else
		{
			MoveTo(Target, DeltaTime);
			bAttacking = false;
			AttackAnimTime = 0;
			break;
		}

	case ATTACK:
		AttackAnimTime += DeltaTime;
		bAttacking = true;
		if (AttackAnimTime >= AttackAnimTimeout)
			AttackAnimTime = 0;
		Rotate();
		break;

	case HIT:
		bAttacking = false;
		break;

	case DIE:
		bAttacking = false;
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

void AEnemy::Rotate()
{
	FVector Dir = Target->GetActorLocation() - GetActorLocation();
	Dir.Normalize();
	FRotator Rotation = Dir.Rotation();
	Rotation.Pitch = 0;

	RootComponent->SetWorldRotation(Rotation);
}

void AEnemy::OnSightOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && eState != RUN)
	{
		bInSight = true;
		Target = OtherActor;
	}
}

void AEnemy::OnAttackRangeOverlapBegin_Implementation(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && eState != ATTACK)
	{
		bInAttackRange = true;
		Target = OtherActor;
	}
}

void AEnemy::OnAttackRangeOverlapEnd_Implementation(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
	{
		bInAttackRange = false;
	}
}

bool AEnemy::IsAttacking()
{
	return bAttacking;
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
			MeleeWeapon->WeaponHolder = this;
		}
	}
}

void AEnemy::SwitchState()
{
	if (!bInSight && !bInAttackRange)
		eState = IDLE;
	else if (bInSight && !bInAttackRange)
		eState = RUN;
	else if (bInSight && bInAttackRange)
		eState = ATTACK;
}

void AEnemy::SwordSwing()
{
	if (MeleeWeapon)
		MeleeWeapon->Swing();
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	DEBUG_d(Log, (int)DamageAmount);

	if (DamageAmount >= 0)
		Hp -= DamageAmount;

	if (Hp <= 0)
	{
		if(MeleeWeapon)
			MeleeWeapon->Destroy();
		
		for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			if (ActorItr->GetName() == TEXT("RespawnPoint"))
			{
				DEBUG(Log, asdlkfjasdlkfja);
			}
		}

		this->SetActorHiddenInGame(true);
	}

	return DamageAmount;
}