// Fill out your copyright notice in the Description page of Project Settings.

#include "RespawnPoint.h"
#include "Enemy.h"
#include "EngineUtils.h"
#include "MeleeWeapon.h"


// Sets default values
ARespawnPoint::ARespawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EnemyCount = 0;
	MaxActiveCount = 11;
	ActiveCount = 0;
	RespawnDelay = 30.f;
	RespawnTime = 0;
}

// Called when the game starts or when spawned
void ARespawnPoint::BeginPlay()
{
	Super::BeginPlay();

	int i = 0;

	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->GetName().Contains(TEXT("Enemy")))
		{
			++i;

			Enemies.Add(Cast<AEnemy>(*ActorItr));
			++EnemyCount;

			if (i > MaxActiveCount)
			{
				Enemies.Last()->SetActorEnableCollision(false);
				Enemies.Last()->SetActorHiddenInGame(true);

				if (Enemies.Last()->GetMeleeWeapon())
				{
					Enemies.Last()->GetMeleeWeapon()->SetActorEnableCollision(false);
					Enemies.Last()->GetMeleeWeapon()->SetActorHiddenInGame(true);
				}

				if (Enemies.Last()->GetRangeWeapon())
				{
					Enemies.Last()->GetRangeWeapon()->SetActorEnableCollision(false);
					Enemies.Last()->GetRangeWeapon()->SetActorHiddenInGame(true);
				}
			}
			else
				++ActiveCount;
		}
	}
}

// Called every frame
void ARespawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActiveCount < MaxActiveCount)
	{
		RespawnTime += DeltaTime;

		if (RespawnTime >= RespawnDelay)
		{
			SpawnEnemy();
			RespawnTime = 0;
		}
	}
}

void ARespawnPoint::SpawnEnemy()
{
	for (int i = 0; i < EnemyCount; ++i)
	{
		if (Enemies[i]->bHidden)
		{
			++ActiveCount;
			Enemies[i]->SetHidden(false);
			break;
		}
	}
}

void ARespawnPoint::DecreaseActiveCount()
{
	--ActiveCount;
}
