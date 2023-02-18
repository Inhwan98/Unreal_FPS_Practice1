// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Perception/AIPerceptionTypes.h"

#include "CoreMinimal.h"
#include "AIController.h"
#include "UndeadAIController.generated.h"

/**
 * 
 */
 // * UCLASS(Config=Game)
 // -환경설정 -> Game 카테고리에서 클래스를 읽을 수 있게 해줌
UCLASS(Config = Game)
class FPSPROJECT_API AUndeadAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AUndeadAIController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		class UAISenseConfig_Sight* SightConfig; //시야 관련 클래스

	UFUNCTION(BlueprintCallable, Category = Behavior)
		void OnTargetDetected(AActor* actor, FAIStimulus const stimulus);

	class UBlackboardComponent* GetBlackBoard() const;
	//블랙보드를 반환하는 함수


private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* btreeComp;
	//AllowPrivateAccess : private변수를 블루프린트에서 접근 허용

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* btree;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBlackboardComponent* blackboardComp;
};
