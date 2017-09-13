// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "macro.h"
#include "Enemy.generated.h"

class AMeleeWeapon;
class ARespawnPoint;
UCLASS()
class CHARLEE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	//Enemy ����
	enum STATE {
		IDLE,
		RUN,
		ATTACK,
		HIT,
		DIE
	};

	STATE eState;

	AActor* Target;

	//Deltatime
	float AttackAnimTime;

	//ü��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyProperties)
	float Hp;

	//������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyProperties)
	float Damage;

	//���� ��� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyProperties)
	float AttackTimeout;

	//���� �ִϸ��̼� ��� �ð�
	float AttackAnimTimeout;

	//�ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyProperties)
	float Speed;

	//�þ�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Collision)
	USphereComponent* SightSphere;

	//���� ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Collision)
	USphereComponent* AttackRangeSphere;

	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyProperties)
	TSubclassOf<AMeleeWeapon> BPMeleeWeapon;

	AMeleeWeapon* MeleeWeapon;

	bool bAttacking;

	bool bInSight;
	
	bool bInAttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyProperties)
	TSubclassOf<ARespawnPoint> BPRespawnPoint;

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
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintNativeEvent, Category = Collision)
	void OnSightOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	virtual void OnSightOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION(BlueprintNativeEvent, Category = Collision)
	void OnAttackRangeOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	virtual void OnAttackRangeOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION(BlueprintNativeEvent, Category = Collision)
	void OnAttackRangeOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	virtual void OnAttackRangeOverlapEnd_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void MoveTo(AActor* OtherActor, float DeltaTime);
	void Rotate();

	UFUNCTION(BlueprintCallable, Category = Collision)
	bool IsAttacking();

	virtual void PostInitializeComponents() override;

	void SwitchState();

	UFUNCTION(BlueprintCallable, Category = Collision)
	void SwordSwing();
};
