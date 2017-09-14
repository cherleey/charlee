// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pistol.generated.h"

UCLASS()
class CHARLEE_API APistol : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APistol();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//¸Þ½¬
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = RangeWeapon)
	UStaticMeshComponent* Hammer;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = RangeWeapon)
	UStaticMeshComponent* Mag;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = RangeWeapon)
	UStaticMeshComponent* Body;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = RangeWeapon)
	UStaticMeshComponent* Slide;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = RangeWeapon)
	UStaticMeshComponent* Trigger;

private:
	float Damage;

public:
	float GetDamage();
	
};
