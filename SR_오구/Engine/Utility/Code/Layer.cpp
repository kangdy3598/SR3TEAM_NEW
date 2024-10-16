#include "Layer.h"

CLayer::CLayer()
	: m_eGameState(GAMESTATE_NONE)
{
}

CLayer::~CLayer()
{
}

CComponent* CLayer::Get_Component(COMPONENTID eID, const _tchar* pObjTag, const _tchar* pComponentTag)
{
	auto iter = find_if(m_mapObejct.begin(), m_mapObejct.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObejct.end())
		return nullptr;

	return iter->second->Get_Component(eID, pComponentTag);
}

HRESULT CLayer::Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject)
{
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	m_mapObejct.insert({ pObjTag, pGameObject });
	
	return S_OK;
}

CGameObject* CLayer::Get_GameObject(const _tchar* pLayeTag, const _tchar* pObjTag)
{	
	auto iter = find_if(m_mapObejct.begin(), m_mapObejct.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObejct.end())
		return nullptr; 

	return iter->second;
}

HRESULT CLayer::Ready_Layer()
{
	return S_OK;
}

void CLayer::LateReady_Layer()
{
	for (auto& pObj : m_mapObejct)
	{
		pObj.second->LateReady_GameObject();
	}
}

_int CLayer::Update_Layer(const _float& fTimeDelta)
{
	_int  iResult(0);

	for (auto& pObj : m_mapObejct)
	{
		iResult = pObj.second->Update_GameObject(fTimeDelta);

		if (iResult & 0x80000000) // iResult가 음수 일때 
			return iResult;
	}

	return iResult;
}

void CLayer::LateUpdate_Layer(const _float& fTimeDelta)
{
	for (auto& pObj : m_mapObejct)
		pObj.second->LateUpdate_GameObject(fTimeDelta);
}

void CLayer::Render_Layer()
{
	for (auto& pObj : m_mapObejct)
		pObj.second->Render_GameObject();
}

CLayer* CLayer::Create()
{
	CLayer* pLayer = new CLayer;

	if (FAILED(pLayer->Ready_Layer()))
	{
		Safe_Release(pLayer);
		MSG_BOX("layer create failed");
		return nullptr;
	}

	return pLayer;
}

void CLayer::Free()
{
	for_each(m_mapObejct.begin(), m_mapObejct.end(), CDeleteMap());
	m_mapObejct.clear();
}
