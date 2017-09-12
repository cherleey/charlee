// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "GunEffect.generated.h"

UCLASS()
class CHARLEE_API AGunEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Effect)
	UParticleSystemComponent* Particle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	float Duration;

	float AliveTime;
	
};
