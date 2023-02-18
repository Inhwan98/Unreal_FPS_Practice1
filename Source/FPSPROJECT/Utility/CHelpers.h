#pragma once
#include "CoreMinimal.h"
#include "UObject/ConstructorHelpers.h"

//�𸮾� ���ӽ����̽�
//��� ������Ʈ�� �Ҽӵ� �ҽ����� ����
class FPSPROJECT_API CHelpers
{
public:
	template<typename T> //�ڷ��� �̸��� �ٲ���
	static void GetAsset(T** OutObject, FString InPath) //���������ͷ� ������ ������ �ͼ� 
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);

		//������ �� ��� �����߿� �������� üũ
		//�����ͻ� ���
		//���� �����Ҷ��� �������� ����
		//������ ���������� �����Դ��� üũ
		verifyf(asset.Succeeded(), L"asset.Succeeded()");

		*OutObject = asset.Object;
	}

	template<typename T> //�ڷ��� �̸��� �ٲ���
	static void GetAssetDynamic(T** OutObject, FString InPath) //���������ͷ� ������ ������ �ͼ� 
	{
		T* obj = Cast<T>(StaticLoadObject(T::StaticClass(), NULL, *InPath));

		//!! : �𸮾󿡼��� obj != null ���� ǥ���̴�.
		verifyf(obj, L"!!obj");

		*OutObject = obj;
	}

	template<typename T> //�⺻������ TSubŬ������ �������̴�.
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath) //���������ͷ� ������ ������ �ͼ� 
	{
		ConstructorHelpers::FClassFinder<T> asset(*InPath);

		//������ �� ��� �����߿� �������� üũ
		//�����ͻ� ���
		//���� �����Ҷ��� �������� ����
		//������ ���������� �����Դ��� üũ
		verifyf(asset.Succeeded(), L"asset.Succeeded()");

		*OutClass = asset.Class;
	}

	template<typename T>
	static void CreateComponent(AActor* InActor, T** InComponent, FName InName, USceneComponent* InParent = NULL)
	{
		*InComponent = InActor->CreateDefaultSubobject<T>(InName);
		if (!!InParent)
		{
			(*InComponent)->SetupAttachment(InParent);
			return;
		}

		InActor->SetRootComponent((*InComponent));
	}
	template<typename T>
	static void FindActors(class UWorld* InWorld, TArray<T*>& OutActors)
	{
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsOfClass(InWorld, T::StaticClass(), actors);
		for (AActor* actor : actors)
		{
			OutActors.Add(Cast<T>(actor));
		}
	}

};