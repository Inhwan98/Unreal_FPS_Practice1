// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSPROJECTGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API AFPSPROJECTGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFPSPROJECTGameModeBase();

public:
    UFUNCTION(BlueprintCallable, Category = "UMG_Game")
        void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG_Game")
        TSubclassOf<UUserWidget> StartingWidgetClass;

    UPROPERTY()
        UUserWidget* CurrentWidget;
};


