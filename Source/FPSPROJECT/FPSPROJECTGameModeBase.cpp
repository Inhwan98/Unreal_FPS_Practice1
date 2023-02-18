// Copyright Epic Games, Inc. All Rights Reserved.

#include "Blueprint/UserWidget.h"
#include "Global.h"
#include "FPSPROJECTGameModeBase.h"

AFPSPROJECTGameModeBase::AFPSPROJECTGameModeBase()
{
	//DefaultPawnClass: �⺻ �÷��̾� Ŭ������ �Է¹��� ����
	//TSubclassOf<APawn>���� ���� ��ӹ��� ���� Ŭ���� Ÿ���� �Է�

	//Blueprint'/Game/CPlayerBP.CPlayerBP'
	//�������Ʈ�� Ŭ����Ÿ���� ������ �ǵڿ� _C�� �� �ٿ���� �Ѵ�.
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