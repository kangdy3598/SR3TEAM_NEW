#include "GameObject.h"


CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDevice)
	:m_pGraphicDev(pGraphicDevice)
	, m_strKey(L"")
	, m_iObjType(0)
	, m_bIsTransparent(false)
	, m_bDontDestroy(false)
{
	m_pGraphicDev->AddRef();
}

CGameObject::CGameObject(const CGameObject& rhs)
	:m_pGraphicDev(rhs.m_pGraphicDev)
	, m_strKey(L"")
	, m_bIsTransparent(false)
	, m_bDontDestroy(false)
{
	m_pGraphicDev->AddRef();
}

CGameObject::~CGameObject()
{
}

CComponent* CGameObject::Get_Component(COMPONENTID eID, const _tchar* pComponentTag)
{
	CComponent* pComponent = Find_Component(eID, pComponentTag);
	//NULL_CHECK_RETURN(pComponent, nullptr);

	return pComponent;
}

HRESULT CGameObject::Ready_GameObject()
{
	return S_OK;
}

void CGameObject::LateReady_GameObject()
{
	for (auto& pComponent : m_mapComponent[ID_STATIC])
		pComponent.second->LateReady_Component();

	for (auto& pComponent : m_mapComponent[ID_DYNAMIC])
		pComponent.second->LateReady_Component();
}

_int CGameObject::Update_GameObject(const _float& fTimeDelta)
{
	for (auto& pComponent : m_mapComponent[ID_DYNAMIC])
		pComponent.second->Update_Component(fTimeDelta);

	return 0;
}

void CGameObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
	for (auto& pComponent : m_mapComponent[ID_DYNAMIC])
		pComponent.second->LateUpdate_Component(fTimeDelta);

}

void CGameObject::Compute_ViewZ(const _vec3* pPos)
{
	_matrix	matCamWorld;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	_vec3		vCamPos;
	memcpy(&vCamPos, &matCamWorld.m[3][0], sizeof(_vec3));

	_vec3		vSub = vCamPos - *pPos;
	m_fViewZ = D3DXVec3Length(&vSub);
}



CComponent* CGameObject::Find_Component(COMPONENTID eID, const _tchar* pComponentTag)
{
	auto iter = find_if(m_mapComponent[eID].begin(), m_mapComponent[eID].end(), CTag_Finder(pComponentTag));

	if (iter == m_mapComponent[eID].end())
		return nullptr;

	return iter->second;
}

void CGameObject::Free()
{
	for (_uint i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);

}


