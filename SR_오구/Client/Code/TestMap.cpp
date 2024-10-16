#include "pch.h"
#include "TestMap.h"
#include "Export_Utility.h"


CTestMap::CTestMap(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
	
}

CTestMap::~CTestMap()
{
}

HRESULT CTestMap::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	return S_OK;
}

_int CTestMap::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_NONALPHA, this);

	return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CTestMap::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTestMap::Render_GameObject()
{



	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	
	//FAILED_CHECK_RETURN(SetUp_Material(), );
	m_pTextureCom->Set_Texture();

	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);



	
}

HRESULT CTestMap::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CStartMap*>(Engine::Clone_Proto(L"Proto_TestMapTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_TerrainTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

void CTestMap::Key_Input(const _float& fTimeDelta)
{
}

CTestMap* CTestMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTestMap* pTestMap = new CTestMap(pGraphicDev);

	if(FAILED(pTestMap->Ready_GameObject()))
	{
		Safe_Release(pTestMap);
		MSG_BOX("pTestMap Create Failed");
		return nullptr;
	}

	return pTestMap;
}

void CTestMap::Free()
{
	Engine::CGameObject::Free();
}
