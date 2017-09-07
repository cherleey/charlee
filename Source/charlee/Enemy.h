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
	//ü��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyProperties)
	float Hp;

	//������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyProperties)
	float Damage;

	//���� ��� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyProperties)
	float AttackTimeout;

	//�ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyProperties)
	float Speed;

	//�þ�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Collision)
	USphereComponent* SightSphere;

	//���� ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Collision)
	USphereComponent* AttackRangeSphere;
};
