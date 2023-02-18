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
 // -ȯ�漳�� -> Game ī�װ����� Ŭ������ ���� �� �ְ� ����
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
		class UAISenseConfig_Sight* SightConfig; //�þ� ���� Ŭ����

	UFUNCTION(BlueprintCallable, Category = Behavior)
		void OnTargetDetected(AActor* actor, FAIStimulus const stimulus);

	class UBlackboardComponent* GetBlackBoard() const;
	//�����带 ��ȯ�ϴ� �Լ�


private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* btreeComp;
	//AllowPrivateAccess : private������ �������Ʈ���� ���� ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* btree;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBlackboardComponent* blackboardComp;
};
