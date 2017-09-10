// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class CHARLEE_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	float CanvasSizeX;
	float CanvasSizeY;
	
public:
	virtual void DrawHUD() override;
	void DrawHealthBar();
};
