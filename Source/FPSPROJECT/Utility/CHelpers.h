#pragma once
#include "CoreMinimal.h"
#include "UObject/ConstructorHelpers.h"

//언리얼 네임스페이스
//어느 프로젝트에 소속된 소스인지 구분
class FPSPROJECT_API CHelpers
{
public:
	template<typename T> //자료형 이름을 바꿔줌
	static void GetAsset(T** OutObject, FString InPath) //이중포인터로 에셋을 가지고 와서 
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);

		//오류가 난 경우 개발중에 오류값을 체크
		//에디터상에 사용
		//실제 서비스할때는 실행하지 않음
		//에셋을 정상적으로 가져왔는지 체크
		verifyf(asset.Succeeded(), L"asset.Succeeded()");

		*OutObject = asset.Object;
	}

	template<typename T> //자료형 이름을 바꿔줌
	static void GetAssetDynamic(T** OutObject, FString InPath) //이중포인터로 에셋을 가지고 와서 
	{
		T* obj = Cast<T>(StaticLoadObject(T::StaticClass(), NULL, *InPath));

		//!! : 언리얼에서는 obj != null 같은 표현이다.
		verifyf(obj, L"!!obj");

		*OutObject = obj;
	}

	template<typename T> //기본적으로 TSub클래스가 포인터이다.
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath) //이중포인터로 에셋을 가지고 와서 
	{
		ConstructorHelpers::FClassFinder<T> asset(*InPath);

		//오류가 난 경우 개발중에 오류값을 체크
		//에디터상에 사용
		//실제 서비스할때는 실행하지 않음
		//에셋을 정상적으로 가져왔는지 체크
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