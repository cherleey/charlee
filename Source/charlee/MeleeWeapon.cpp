// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeWeapon.h"
#include "Enemy.h"


// Sets default values
AMeleeWeapon::AMeleeWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Damage = 5;
	bSwinging = false;
	WeaponHolder = NULL;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnHitBoxOverlapBegin);
	HitBox->AttachTo(RootComponent);

}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMeleeWeapon::OnHitBoxOverlapBegin_Implementation(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp != OtherActor->GetRootComponent())
		return;

	if (bSwinging && OtherActor != WeaponHolder && !ThingsHit.Contains(OtherActor))
	{		
		OtherActor->TakeDamage(Damage + WeaponHolder->Damage, FDamageEvent(), this->GetInstigatorController(), this);
		ThingsHit.Add(OtherActor);
	}
}

void AMeleeWeapon::Swing()
{
	ThingsHit.Empty();
	bSwinging = true;
}

void AMeleeWeapon::Rest()
{
	ThingsHit.Empty();
	bSwinging = false;
}
