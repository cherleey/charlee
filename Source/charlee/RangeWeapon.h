// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RangeWeapon.generated.h"

UCLASS()
class CHARLEE_API ARangeWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARangeWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//¸Þ½¬
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = RangeWeapon)
	UStaticMeshComponent* Mesh;
	
};
