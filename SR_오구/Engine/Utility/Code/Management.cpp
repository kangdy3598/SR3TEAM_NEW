#include "Management.h"
#include "Export_Utility.h"

IMPLEMENT_SINGLETON(CManagement)



//:m_pScene(nullptr)

CManagement::CManagement()
	:m_pScene(nullptr)
	,m_imap_stage(0)
{

}

CManagement::~CManagement()
{
	Free();
}

CComponent* CManagement::Get_Component(COMPONENTID eID, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag)
{
	NULL_CHECK_RETURN(m_pScene, nullptr);

	return m_pScene->Get_Component(eID, pLayerTag, pObjTag, pComponentTag);

}

CGameObject* CManagement::Get_GameObject(const _tchar* pLayeTag, const _tchar* pObjTag)
{
	NULL_CHECK_RETURN(m_pScene, nullptr);

	return m_pScene->Get_GameObject(pLayeTag, pObjTag);
}

CLayer* CManagement::Get_Layer(const _tchar* pLayerTag)
{
	NULL_CHECK_RETURN(m_pScene, nullptr);

	return m_pScene->Get_Layer(pLayerTag);
}

HRESULT CManagement::Set_Scene(CScene* pScene)
{
	map<const _tchar*, CGameObject*> dontDestroyEnvironmentMap;
	map<const _tchar*, CGameObject*> dontDestroyGameLogicMap;
	map<const _tchar*, CGameObject*> dontDestroyUIMap;
	if (m_pScene)
	{
		if (m_pScene->Get_Layer(L"Layer_Environment"))
		{
			for (auto& iter : m_pScene->Get_Layer(L"Layer_Environment")->GetLayerGameObjectPtr())
			{
				if (iter.second->GetDontDestroy())
				{
					dontDestroyEnvironmentMap.insert({ iter.first, iter.second });
				}
			}
		}

		if (m_pScene->Get_Layer(L"Layer_GameLogic"))
		{
			for (auto& iter : m_pScene->Get_Layer(L"Layer_GameLogic")->GetLayerGameObjectPtr())
			{
				if (iter.second->GetDontDestroy())
				{
					dontDestroyGameLogicMap.insert({ iter.first, iter.second });
				}
			}
		}

		if (m_pScene->Get_Layer(L"Layer_UI"))
		{
			for (auto& iter : m_pScene->Get_Layer(L"Layer_UI")->GetLayerGameObjectPtr())
			{
				if (iter.second->GetDontDestroy())
				{
					dontDestroyUIMap.insert({ iter.first, iter.second });
				}
			}
		}
	}

	Safe_Release(m_pScene);

	Engine::Clear_RenderGroup();

	m_pScene = pScene;
	m_pScene->Ready_Scene();
	m_pScene->DontDestroy_Layer(L"Layer_Environment", dontDestroyEnvironmentMap);
	m_pScene->DontDestroy_Layer(L"Layer_GameLogic", dontDestroyGameLogicMap);
	m_pScene->DontDestroy_Layer(L"Layer_UI", dontDestroyUIMap);
	m_pScene->init(); // 맵툴에서 가져온 오브젝트들을 위해 사용 
	m_pScene->LateReady_Scene();

	return S_OK;
}

void CManagement::LateReady_Scene()
{
	m_pScene->LateReady_Scene();
}

_int CManagement::Update_Scene(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pScene, -1);

	return m_pScene->Update_Scene(fTimeDelta);
}

void CManagement::LateUpdate_Scene(const _float& fTimeDelta)
{
	NULL_CHECK(m_pScene);
	m_pScene->LateUpdate_Scene(fTimeDelta);
}

void CManagement::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Engine::Render_GameObject(pGraphicDev);

	NULL_CHECK(m_pScene);
	m_pScene->Render_Scene();
}

void CManagement::Free()
{
	Safe_Release(m_pScene);
}
