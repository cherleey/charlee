// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHUD.h"
#include "Avatar.h"
#include "Engine/Canvas.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void AMyHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawHealthBar();
}

void AMyHUD::DrawHealthBar()
{
	AAvatar* avatar = Cast<AAvatar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	float Width = 200;
	float Height = 50;
	float Pad = 12;
	float Margin = 50;
	float PercHp = avatar->Hp / avatar->MaxHp;

	DrawRect(FLinearColor(0, 0, 0, 1), Canvas->SizeX - Width - Pad - Margin, Canvas->SizeY - Height - Pad - Margin, Width + 2 * Pad, Height + 2 * Pad);
	DrawRect(FLinearColor(1 - PercHp, PercHp, 0, 1), Canvas->SizeX - Width - Margin, Canvas->SizeY - Height - Margin, Width * PercHp, Height);
}