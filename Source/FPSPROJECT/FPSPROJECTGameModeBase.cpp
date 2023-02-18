// Copyright Epic Games, Inc. All Rights Reserved.

#include "Blueprint/UserWidget.h"
#include "Global.h"
#include "FPSPROJECTGameModeBase.h"

AFPSPROJECTGameModeBase::AFPSPROJECTGameModeBase()
{
	//DefaultPawnClass: 기본 플레이어 클래스를 입력받을 변수
	//TSubclassOf<APawn>으로 부터 상속받은 하위 클래스 타입을 입력

	//Blueprint'/Game/CPlayerBP.CPlayerBP'
	//블루프린트의 클래스타입을 쓸때는 맨뒤에 _C를 꼭 붙여줘야 한다.
	ConstructorHelpers::FClassFinder<APawn> pawn(L"Blueprint'/Game/BluePrint/BP_CPlayer.BP_CPlayer_C'");
	verifyf(pawn.Succeeded(), L"pawn.Succeeded()");
	DefaultPawnClass = pawn.Class;
}

void AFPSPROJECTGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	ChangeMenuWidget(StartingWidgetClass);
}



void AFPSPROJECTGameModeBase::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
    if (CurrentWidget != nullptr)
    {
        CurrentWidget->RemoveFromViewport();
        CurrentWidget = nullptr;
    }

    if (NewWidgetClass != nullptr)
    {
        CurrentWidget = CreateWidget(GetWorld(), NewWidgetClass);
        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
        }
    }
}