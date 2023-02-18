// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_CrossHair.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UCUserWidget_CrossHair : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnFocus();
	UFUNCTION(BlueprintImplementableEvent)
		void OffFocus();
	
};
