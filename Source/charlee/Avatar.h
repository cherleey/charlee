// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "macro.h"
#include "GunEffect.h"
#include "RangeWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Avatar.generated.h"

UCLASS()
class CHARLEE_API AAvatar : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AAvatar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void MoveVertical(float amount);
	void MoveHorizontal(float amount);
	void Yaw(float amount);
	void Pitch(float amount);
	void MouseLeftClickPressed();
	void MouseLeftClickReleased();
	bool RayCast();
	void PostInitializeComponents();
	
public:
	//마우스 감도
	float MouseSpeed;

	//체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarProperties)
	float MaxHp;
	float Hp;

	//데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarProperties)
	float Damage;

	//공격 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarProperties)
	float AttackTimeout;
	float AttackTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	TSubclassOf<AGunEffect> BPGunEffect;

	//무기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarProperties)
	TSubclassOf<ARangeWeapon> BPRangeWeapon;

	ARangeWeapon* RangeWeapon;

	bool bTraced;
	bool bMouseLeftPressed;
	FVector ImpactPoint;
	AActor* Target;
	float PitchAmount;
};
