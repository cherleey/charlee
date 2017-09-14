// Fill out your copyright notice in the Description page of Project Settings.

#include "Pistol.h"


// Sets default values
APistol::APistol()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	RootComponent = Body;

	Hammer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hammer"));
	Mag = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mag"));
	Slide = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Slide"));
	Trigger = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trigger"));

	Damage = 10.f;
}

// Called when the game starts or when spawned
void APistol::BeginPlay()
{
	Super::BeginPlay();

	Hammer->AttachTo(RootComponent);
	Mag->AttachTo(RootComponent);
	Slide->AttachTo(RootComponent);
	Trigger->AttachTo(RootComponent);
}

// Called every frame
void APistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float APistol::GetDamage()
{
	return Damage;
}