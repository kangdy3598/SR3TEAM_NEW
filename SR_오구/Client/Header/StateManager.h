#pragma once
#include "GameObject.h"
class CStateManager
{
public:

	CGameObject* ob = nullptr;

	CStateManager() : ob(nullptr){}
	virtual ~CStateManager() {}

	virtual void Enter() = 0;
	virtual void Update(const _float& fTimeDelta) = 0;
	virtual void Exit() = 0;
};