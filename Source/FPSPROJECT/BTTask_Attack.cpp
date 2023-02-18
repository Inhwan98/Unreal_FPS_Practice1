// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatInterface.h"
#include "Undead.h"
#include "AIController.h"

#include "BTTask_Attack.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("MeeleAttack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AAIController* const Cont = OwnerComp.GetAIOwner();
	AUndead* const Enemy = Cast<AUndead>(Cont->GetPawn());
	if (ICombatInterface* const Combat = Cast<ICombatInterface>(Enemy))
	{
		if (AttackFinish(Enemy))
		{
			Combat->Execute_MeeleAttack(Enemy);
			//MeeleAttack�Լ� ȣ��
			//ȣ���Ҷ��� Execute�� �����ش�.
		}
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
	//�������̱� ������ �Ұ�ȣ ����(Succeeded)
}

bool UBTTask_Attack::AttackFinish(AUndead * const Enemy)
{
	bool isFinish = Enemy->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(Enemy->GetCurrentMontage());

	return isFinish;
}
