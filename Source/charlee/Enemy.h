// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Enemy.generated.h"

UCLASS()
class CHARLEE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	//체력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyProperties)
	float Hp;

	//데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyProperties)
	float Damage;

	//공격 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyProperties)
	float AttackTimeout;

	//속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyProperties)
	float Speed;

	//시야
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Collision)
	USphereComponent* SightSphere;

	//공격 범위
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Collision)
	USphereComponent* AttackRangeSphere;
};
