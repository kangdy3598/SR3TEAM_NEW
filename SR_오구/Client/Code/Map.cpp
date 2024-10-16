#include "pch.h"
#include "Export_Utility.h"
#include "Map.h"
#include "ImGuiManger.h"


CMap::CMap(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{

}

CMap::~CMap()
{

}

HRESULT CMap::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CMap::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_NONALPHA, this);

	return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CMap::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMap::Render_GameObject()
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	m_pTextureCom->Set_Texture(0);
	//FAILED_CHECK_RETURN(SetUp_Material(), );


	if (CImGuiManger::GetInstance()->GetWireFrameOnOFF())
	{
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}

	if (!CImGuiManger::GetInstance()->GetWireFrameOnOFF())
	{
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}



	if(CImGuiManger::GetInstance()->GetImageButtonClick())
	{
		m_pGraphicDev->SetTexture(0, CImGuiManger::GetInstance()->GetCurrentTerrainTexture());
	}
	else
	{
		m_pTextureCom->Set_Texture();
	}
	

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMap::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CMapTex*>(Engine::Clone_Proto(L"Proto_MapTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_StartMap"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture",pComponent });


	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });





	return S_OK;
}

HRESULT CMap::SetUp_Material()
{
	return S_OK;
}

void CMap::Key_Input(const _float& fTimeDelta)
{
}

CMap* CMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMap* pMap = new CMap(pGraphicDev);

	if(FAILED(pMap->Ready_GameObject()))
	{
		Safe_Release(pMap);
		MSG_BOX("pMap Create Failed");
		return nullptr;
	}

	return pMap;
}

void CMap::Free()
{
	Engine::CGameObject::Free();

}
