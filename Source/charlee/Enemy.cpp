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

	RespawnPoint = NULL;
	MeleeWeapon = NULL;
	RangeWeapon = NULL;
	BPMeleeWeapon = NULL;
	BPRangeWeapon = NULL;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AttackAnimTime = 1.5f;
	MaxHp = 100.f;
	Hp = MaxHp;
	Damage = 1;
	AttackTimeout = 0;
	RangeAttackTimeout = 2.3f;
	AttackAnimTimeout = 1.5f;
	RangeAttackAnimTimeout = 1.f;
	eState = IDLE;
	Target = NULL;
	RangeTarget = NULL;
	bAttacking = false;
	bInSight = false;
	bInAttackRange = false;
	OriginLocation = this->GetActorLocation();

	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->GetName().Contains(TEXT("Respawn")))
		{
			RespawnPoint = Cast<ARespawnPoint>(*ActorItr);
		}
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SwitchState(DeltaTime);
	CheckAlly();

	if (RangeWeapon && bAttacking)
	{
		if (!RayCast() || Target->GetName().Contains(TEXT("Enemy")))
			return;

		RangeAttackTime += DeltaTime;

		if (RangeAttackTime >= RangeAttackTimeout)
		{
			AGunEffect* Effect = GetWorld()->SpawnActor<AGunEffect>(BPGunEffect, ImpactPoint, FRotator(0));
			RangeTarget->TakeDamage(Damage + RangeWeapon->GetDamage(), FDamageEvent(), this->GetInstigatorController(), this);
			RangeAttackTime = 0;
		}
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
	if (OtherActor->GetName().Contains(TEXT("Enemy")))
		Ally.Add(Cast<AEnemy>(OtherActor));

	if (!OtherActor->GetName().Contains(TEXT("Avatar")))
		return;

	if (OtherActor != NULL && OtherActor != this && OtherComp != NULL && eState != RUN)
	{
		bInSight = true;
		Target = OtherActor;
	}
}

void AEnemy::OnAttackRangeOverlapBegin_Implementation(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (!OtherActor->GetName().Contains(TEXT("Avatar")))
		return;

	if (OtherActor != NULL && OtherActor != this && OtherComp != NULL && eState != ATTACK)
	{
		bInAttackRange = true;
		Target = OtherActor;
	}
}

void AEnemy::OnAttackRangeOverlapEnd_Implementation(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor->GetName().Contains(TEXT("Avatar")))
		return;

	if (OtherActor != NULL && OtherActor != this && OtherComp != NULL)
		bInAttackRange = false;
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
		Speed = 250.f;

		MeleeWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(BPMeleeWeapon);
		
		if (BPMeleeWeapon)
		{
			const USkeletalMeshSocket* socket = this->GetMesh()->GetSocketByName("RightHandSocket");
			socket->AttachActor((AActor*)MeleeWeapon, this->GetMesh());
			MeleeWeapon->WeaponHolder = this;
		}
	}

	if (BPRangeWeapon)
	{
		Speed = 100.f;

		RangeWeapon = GetWorld()->SpawnActor<APistol>(BPRangeWeapon, this->GetActorLocation(), this->GetActorRotation());

		if (BPRangeWeapon)
		{
			const USkeletalMeshSocket* socket = this->GetMesh()->GetSocketByName("RightHandSocketForRange");
			socket->AttachActor((AActor*)RangeWeapon, this->GetMesh());
		}
	}
}

void AEnemy::SwitchState(float DeltaTime)
{
	if (!bInSight && !bInAttackRange && !Target)
		eState = IDLE;
	else if (bInSight && !bInAttackRange)
	{
		eState = RUN;
		DEBUG(Log, asdf);
	}
	else if (bInSight && bInAttackRange)
		eState = ATTACK;

	switch (eState)
	{
	case IDLE:
		MoveTo(Target, DeltaTime);
		bAttacking = false;
		break;

	case RUN:
		if (MeleeWeapon)
		{
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
		}
		else
		{
			if (AttackAnimTime < RangeAttackAnimTimeout && bAttacking)
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
		}

	case ATTACK:
		if (MeleeWeapon)
		{
			AttackAnimTime += DeltaTime;
			bAttacking = true;
			if (AttackAnimTime >= AttackAnimTimeout)
				AttackAnimTime = 0;
			Rotate();
			break;
		}
		else
		{
			AttackAnimTime += DeltaTime;
			bAttacking = true;
			if (AttackAnimTime >= RangeAttackAnimTimeout)
				AttackAnimTime = 0;
			Rotate();
			break;
		}

	case HIT:
		bAttacking = false;
		break;

	case DIE:
		bAttacking = false;
		break;
	}
}

void AEnemy::SwordSwing()
{
	if (MeleeWeapon)
		MeleeWeapon->Swing();
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (DamageAmount >= 0)
	{
		if (!Target)
			Target = DamageCauser;

		Hp -= DamageAmount;
	}

	if (Hp <= 0)
		SetHidden(true);

	return DamageAmount;
}

void AEnemy::SetHidden(bool bHidden)
{
	this->SetActorEnableCollision(!bHidden);
	this->SetActorHiddenInGame(bHidden);

	if(MeleeWeapon)
	{ 
		MeleeWeapon->SetActorEnableCollision(!bHidden);
		MeleeWeapon->SetActorHiddenInGame(bHidden);
	}

	if (RangeWeapon)
	{
		RangeWeapon->SetActorEnableCollision(!bHidden);
		RangeWeapon->SetActorHiddenInGame(bHidden);
	}

	if (bHidden)
	{
		Target = NULL;
		Hp = MaxHp;
		this->SetActorLocation(OriginLocation);
		RespawnPoint->DecreaseActiveCount();
		eState = IDLE;
		bInAttackRange = false;
		bInSight = false;
		Ally.Empty();
	}
}

AEnemy::STATE AEnemy::GetState()
{
	return eState;
}

void AEnemy::CheckAlly()
{
	if (Ally.Num() == 0 || Target)
		return;

	auto iter = Ally.CreateIterator();

	for (; iter; ++iter)
	{
		if (eState == IDLE)
			Target = (*iter)->GetTarget();
	}
}

AActor * AEnemy::GetTarget()
{
	return Target;
}

AMeleeWeapon * AEnemy::GetMeleeWeapon()
{
	return MeleeWeapon;
}

APistol* AEnemy::GetRangeWeapon()
{
	return RangeWeapon;
}

bool AEnemy::RayCast()
{
	FHitResult RV_Hit(ForceInit);
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);

	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = true;
	RV_TraceParams.AddIgnoredActor(this);

	FVector Start = this->GetActorLocation() + (this->GetActorRotation().Vector());
	FVector End = this->GetActorLocation() + (this->GetActorRotation().Vector() * 10000);

	bool bTraced = GetWorld()->LineTraceSingleByChannel(RV_Hit, Start, End, ECC_Pawn, RV_TraceParams);

	if (bTraced)
	{
		RangeAttackTime += GetWorld()->DeltaTimeSeconds;

		ImpactPoint = RV_Hit.ImpactPoint;
		RangeTarget = RV_Hit.GetActor();
	}
	
	return bTraced;
}

bool AEnemy::IsMelee()
{
	if (MeleeWeapon)
		return true;
	else
		return false;
}