// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "SwordEffect.generated.h"

UCLASS()
class CHARLEE_API ASwordEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwordEffect();

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

private:
	float AliveTime;
	AActor* Root;

public:
	void DrawEffect(AActor* actor, float duration);
};
