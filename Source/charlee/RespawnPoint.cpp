// Fill out your copyright notice in the Description page of Project Settings.

#include "RespawnPoint.h"
#include "Enemy.h"


// Sets default values
ARespawnPoint::ARespawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EnemyCount = 10;
	MaxActiveCount = 3;
	ActiveCount = 0;
	RespawnDelay = 2.f;
	RespawnTime = 0;
}

// Called when the game starts or when spawned
void ARespawnPoint::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < EnemyCount; ++i)
	{
		//AEnemy* enemy = GetWorld()->SpawnActor<AEnemy>(BPEnemy, FVector(rand() % 100, rand() % 100, 0), FRotator(0));
		//enemy->SetActorHiddenInGame(true);

		//Enemies[i] = enemy;
	}
}

// Called every frame
void ARespawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*if (ActiveCount < MaxActiveCount)
	{
		RespawnTime += DeltaTime;

		if (RespawnTime >= RespawnDelay)
		{
			SpawnEnemy();
			RespawnTime = 0;
		}
	}*/
}

void ARespawnPoint::SpawnEnemy()
{
	for (int i = 0; i < EnemyCount; ++i)
	{
		if (Enemies[i]->bHidden)
		{
			
			++ActiveCount;
			//Enemies[i]->SetActorLocation(FVector(rand() % 100, rand() % 100, 0));
			//Enemies[i]->SetActorHiddenInGame(false);
			break;
		}
	}
}

void ARespawnPoint::DecreaseActiveCount()
{
	--ActiveCount;
}
