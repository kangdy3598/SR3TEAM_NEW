#pragma once

#include "Collider.h"
#include "GameObject.h"


BEGIN(Engine)

union COLLIDER_ID
{
	struct
	{
		UINT Left_id;
		UINT Right_id;
	};
	ULONGLONG ID;
};


class ENGINE_DLL CCollisionMgr
{
	DECLARE_SINGLETON(CCollisionMgr)

private:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr();

public:
	void init();
	void update();
	void Lateupdate();

private:
	map<ULONGLONG, bool>	 m_mapColInfo; //충돌체 간의 이전 프레임 충돌 정보
	UINT					 m_arrCheck[(UINT)GROUP_TYPE::GROUP_END]; // 32바이트 32개. uint는 4바이트이브로 32비트

public:
	void CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);

	void Reset()
	{
		memset(m_arrCheck, 0, sizeof(UINT) * (UINT)GROUP_TYPE::GROUP_END);
	}

private:
	void CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	bool IsCollision(CComponent* _pLeftCol, CComponent* _pRightCol);

};

END