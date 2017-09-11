// Fill out your copyright notice in the Description page of Project Settings.

#include "SwordEffect.h"


// Sets default values
ASwordEffect::ASwordEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	AliveTime = 0;
	Duration = 0.5f;
}

// Called when the game starts or when spawned
void ASwordEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASwordEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AliveTime += DeltaTime;

	if (AliveTime >= Duration)
		Destroy();
}

