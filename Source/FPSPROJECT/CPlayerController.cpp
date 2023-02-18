// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerController.h"

void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameAndUI());
}