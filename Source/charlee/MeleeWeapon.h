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
	//������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MeleeWeapon)
	float Damage;

	//���⸦ �ֵθ��� �ִ���
	bool bSwinging;

	//���⿡ �浹�� ���͵��� �����ϴ� �迭
	TArray<AActor*> ThingsHit;

	//���⸦ ��� �ִ� ����
	AEnemy* WeaponHolder;

	//��Ʈ�ڽ�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = MeleeWeapon)
	UBoxComponent* HitBox;

	//�޽�
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
