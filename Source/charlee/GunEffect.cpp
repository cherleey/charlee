// Fill out your copyright notice in the Description page of Project Settings.

#include "GunEffect.h"


// Sets default values
AGunEffect::AGunEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	AliveTime = 0;
	Duration = 0.5f;
}

// Called when the game starts or when spawned
void AGunEffect::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGunEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AliveTime += DeltaTime;

	if (AliveTime >= Duration)
		Destroy();
}
