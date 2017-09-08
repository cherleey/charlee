// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "MeleeWeapon.generated.h"

class AEnemy;
UCLASS()
class CHARLEE_API AMeleeWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	//데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeleeWeapon)
	float Damage;

	//무기를 휘두르고 있는지
	bool bSwinging;

	//무기에 충돌한 엑터들을 저장하는 배열
	TArray<AActor*> ThingsHit;

	//무기를 들고 있는 엑터
	AEnemy* WeaponHolder;

	//히트박스
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MeleeWeapon)
	UBoxComponent* HitBox;

	//메쉬
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MeleeWeapon)
	UStaticMeshComponent* Mesh;

public:	
	// Sets default values for this actor's properties
	AMeleeWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintNativeEvent, Category = Collision)
	void OnHitBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	void Swing();
	void Rest();
};
