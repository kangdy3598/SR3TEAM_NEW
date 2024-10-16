#include "pch.h"
#include "Export_Utility.h"
#include "JungleForestMap.h"


CJungleForestMap::CJungleForestMap(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CJungleForestMap::~CJungleForestMap()
{
}

HRESULT CJungleForestMap::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CJungleForestMap::Update_GameObject(const float& fTimeDelta)
{
	Add_RenderGroup(RENDER_NONALPHA, this);

	return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CJungleForestMap::LateUpdate_GameObject(const float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CJungleForestMap::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CJungleForestMap::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CMapTex*>(Engine::Clone_Proto(L"Proto_JungleMonkeyTownMapTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_JungleMonkeyTownMap"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture",pComponent });


	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });


	return S_OK;
}

HRESULT CJungleForestMap::SetUp_Material()
{
	return S_OK;
}

CJungleForestMap* CJungleForestMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CJungleForestMap* pJungleForestMap = new CJungleForestMap(pGraphicDev);

	if (FAILED(pJungleForestMap->Ready_GameObject()))
	{
		Safe_Release(pJungleForestMap);
		MSG_BOX("pMap Create Failed");
		return nullptr;
	}

	return pJungleForestMap;
}

void CJungleForestMap::Free()
{
	Engine::CGameObject::Free();
}
