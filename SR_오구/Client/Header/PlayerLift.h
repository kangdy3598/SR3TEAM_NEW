#pragma once
#include "State.h"
#include "Player.h"
#include "Export_Utility.h"

class PlayerLift : public CState
{
public:

	static PlayerLift* GetInstance()
	{
		if (!m_instance)
			m_instance = new PlayerLift;

		return m_instance;
	}
	static void	DestroyInstance()
	{
		if (m_instance)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}

	virtual void Enter();
	virtual void Update(const _float& fTimeDelta);
	virtual void Exit();
	virtual void Free() {}

private:
	static PlayerLift* m_instance;
	int m_iStateCount = 0;

	float timeElapsed = 0.0f;

	CGameObject* colObj = nullptr;
	CTransform* colObjTransform = nullptr;
	_vec3 vColPos;
	_vec3 vDownPos;

private:
	void MoveAlongBezierCurve(
		float deltaTime, _vec3& currentPosition, const _vec3& A, const _vec3& B,
		_bool isLift = true);

	_vec3 Lerp(const _vec3& a, const _vec3& b, float t)
	{
		return _vec3(
			(1 - t) * a.x + t * b.x,
			(1 - t) * a.y + t * b.y,
			(1 - t) * a.z + t * b.z
		);
	}

	_vec3 QuadraticBezier(const _vec3& P0, const _vec3& P1, const _vec3& P2, float t)
	{
		_vec3 L1 = Lerp(P0, P1, t);
		_vec3 L2 = Lerp(P1, P2, t);
		return Lerp(L1, L2, t);
	}

};