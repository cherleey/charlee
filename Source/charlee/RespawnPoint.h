// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RespawnPoint.generated.h"

class AEnemy;
UCLASS()
class CHARLEE_API ARespawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARespawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	TArray<AEnemy*> Enemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AvatarProperties)
	TSubclassOf<AEnemy> BPEnemy;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyCount)
	int EnemyCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyCount)
	int MaxActiveCount;

	int ActiveCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyCount)
	float RespawnDelay;

	float RespawnTime;

public:
	void SpawnEnemy();
	void DecreaseActiveCount();
};
