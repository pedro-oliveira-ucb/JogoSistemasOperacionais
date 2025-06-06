#pragma once
#include <chrono>

#include "../CBaseAttack/CBaseAttack.h"

class CRangedAttack : public CBaseAttack
{
	void otherActiveLogic( CBaseEntity * sender ) override;
	void otherDeactiveLogic( ) override;
	double lastUpdateTime;

public:
	CRangedAttack( CBaseEntityConstructor entityBuilder ,
		CBaseAttackConstructor attackBuilder );
	
	void updateEntity( ) override; // Atualiza a entidade

	std::shared_ptr<CBaseAttack> Clone( ) override;
};

