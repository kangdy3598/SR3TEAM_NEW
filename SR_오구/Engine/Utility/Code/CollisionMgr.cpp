#include "CollisionMgr.h"
#include "Management.h"


IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
	:m_arrCheck{}
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::init()
{
	Reset();
}

void CCollisionMgr::update()
{

	// 체크 된 값 즉 1이 있는지 검사하는 코드

	//행 32번 검사 


	for (UINT iRow = 0; iRow < (UINT)GROUP_TYPE::GROUP_END; ++iRow)
	{

		for (UINT iCol = iRow; iCol < (UINT)GROUP_TYPE::GROUP_END; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				//여기 중단점 걸렸을때가 이제 1인지점
				//CManagement::GetInstance()->GetCurScenePtr()->GetLayerMapPtr()
				CollisionGroupUpdate((GROUP_TYPE)iRow, (GROUP_TYPE)iCol);
			}
		}
	}

}

void CCollisionMgr::Lateupdate()
{

}

void CCollisionMgr::CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	// 더 작은 값의 그룹 타입을 행으로
	// 큰 값을 열(비트)로 사용

	UINT iRow = (UINT)_eLeft;
	UINT iCol = (UINT)_eRight;

	if (iCol < iRow)
	{
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol))
	{
		m_arrCheck[iRow] &= ~(1 << iCol);
	}

	else
	{
		m_arrCheck[iRow] |= (1 << iCol);
	}

}

void CCollisionMgr::CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	const vector<CGameObject*>& vecLeft = CManagement::GetInstance()->GetCurScenePtr()->GetVecObjlist(_eLeft);
	const vector<CGameObject*>& vecRight = CManagement::GetInstance()->GetCurScenePtr()->GetVecObjlist(_eRight);
	map<ULONGLONG, bool>::iterator iter;	

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{

		// 충돌체를 보유하지 않은 경우 
		if (vecLeft[i]->Find_Component(ID_DYNAMIC, L"Com_Collider") == nullptr || !vecLeft[i]->Is_Active())
			continue;

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			// 충돌체가 없거나, 자기 자신과의 충돌인 경우 
			if (nullptr == vecRight[j]->Find_Component(ID_DYNAMIC, L"Com_Collider") || !vecRight[j]->Is_Active()
				|| vecLeft[i] == vecRight[j])
			{
				continue;
			}


			// 두 충돌체 조합 아이디 생성
			COLLIDER_ID ID;
			ID.Left_id  = dynamic_cast<CCollider*>(vecLeft[i]->Find_Component(ID_DYNAMIC, L"Com_Collider"))->GetID();
			ID.Right_id = dynamic_cast<CCollider*>(vecRight[j]->Find_Component(ID_DYNAMIC, L"Com_Collider"))->GetID();
			ID.ID; // 이걸 맵의 키값으로 사용 

			iter = m_mapColInfo.find(ID.ID);			

			// 충돌 정보가 미 등록 상태인 경우 등록 ( 충돌하지 않았다로) 즉 처음 맵에 insert 되는경우 
			// 이미 insert 되어 있으면 건너뜀
			if(m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}


			if (IsCollision(vecLeft[i]->Find_Component(ID_DYNAMIC, L"Com_Collider"), vecRight[j]->Find_Component(ID_DYNAMIC, L"Com_Collider")))
			{
				//현재 충돌 할 경우
				
				if(iter->second)
				{
					// 지금도 충돌중인데 이전에도 충돌하고 있는 경우
					vecLeft[i]->OnCollision(vecRight[j]);
					vecRight[j]->OnCollision(vecLeft[i]);
				}
				else
				{
					// 이전에는 충돌하지 않았다.
					vecLeft[i]->OnCollisionEnter(vecRight[j]);
					vecRight[j]->OnCollisionEnter(vecLeft[i]);
					iter->second = true;
				}
			}

			else
			{
				//현재 충돌 안할 경우 
				if(iter->second)
				{
					// 이전에는 충돌하고 있었다.
					vecLeft[i]->OnCollisionExit(vecRight[j]);
					vecRight[j]->OnCollisionExit(vecLeft[i]);
					iter->second = false;
				}
			}

		}
	}
}

bool CCollisionMgr::IsCollision(CComponent* _pLeftCol, CComponent* _pRightCol)
{

	CCollider* pLeftBoundBox = dynamic_cast<CCollider*>(_pLeftCol);
	CCollider* pRightBoundBox = dynamic_cast<CCollider*>(_pRightCol);

	if (pLeftBoundBox->GetWorldVecMin().x <= pRightBoundBox->GetWorldVecMax().x
		&& pLeftBoundBox->GetWorldVecMax().x >= pRightBoundBox->GetWorldVecMin().x
		&& pLeftBoundBox->GetWorldVecMin().y <= pRightBoundBox->GetWorldVecMax().y
		&& pLeftBoundBox->GetWorldVecMax().y >= pRightBoundBox->GetWorldVecMin().y
		&& pLeftBoundBox->GetWorldVecMin().z <= pRightBoundBox->GetWorldVecMax().z
		&& pLeftBoundBox->GetWorldVecMax().z >= pRightBoundBox->GetWorldVecMin().z)
	{
		pLeftBoundBox->m_bisCollision = true;
		pRightBoundBox->m_bisCollision = true;
		return true;
	}


	return false;
}


