#include "pch.h"
#include "Terrain.h"
#include "Export_Utility.h"


CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
{
}

CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);   

    return S_OK;
}

_int CTerrain::Update_GameObject(const _float& fTimeDelta)
{
    //Key_Input(fTimeDelta);

    Add_RenderGroup(RENDER_NONALPHA, this);

    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CTerrain::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTerrain::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// 와이어프레임 모드 활성화
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


	FAILED_CHECK_RETURN(SetUp_Material(), );
    m_pTextureCom->Set_Texture();

    m_pBufferCom->Render_Buffer();

	// 솔리드 모드 활성화 (기본 설정)
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CTerrain::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CTerrainTex*>(Engine::Clone_Proto(L"Proto_TerrainTex"));
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

HRESULT CTerrain::SetUp_Material()
{
	D3DMATERIAL9  tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = { 1.f, 1.f, 1.f, 1.f };
	tMtrl.Specular = { 1.f, 1.f, 1.f, 1.f };
	tMtrl.Ambient = { 0.2f, 0.2f, 0.2f, 1.f };

	tMtrl.Emissive = { 0.f, 0.f, 0.f, 0.f };
	tMtrl.Power = 0.f;

	m_pGraphicDev->SetMaterial(&tMtrl);

	return S_OK;
}

void CTerrain::Key_Input(const _float& fTimeDelta)
{
	//_vec3	vUp;
	//_vec3 vRight;
	//
	//
	//m_pTransformCom->Get_Info(INFO_UP, &vUp);
	//m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	//
	//if (GetAsyncKeyState(VK_UP))
	//{
	//	m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vUp, &vUp), fTimeDelta, 10.f);
	//}
	//
	//if (GetAsyncKeyState(VK_DOWN))
	//{
	//	m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vUp, &vUp), fTimeDelta, -10.f);
	//}
	//
	//if (GetAsyncKeyState(VK_LEFT))
	//{
	//	m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vRight, &vRight), fTimeDelta, -10.f);
	//}
	//
	//
	//if (GetAsyncKeyState(VK_RIGHT))
	//{
	//	m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vRight, &vRight), fTimeDelta, 10.f);
	//}
	//
	//if (GetAsyncKeyState('Q'))
	//{
	//	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(180.f * fTimeDelta));
	//}
	//
	//if (GetAsyncKeyState('A'))
	//{
	//	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(180.f * fTimeDelta));
	//}
	//
	//if (GetAsyncKeyState('W'))
	//{
	//	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
	//}
	//
	//if (GetAsyncKeyState('S'))
	//{
	//	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
	//}
	//
	//if (GetAsyncKeyState('E'))
	//{
	//	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(180.f * fTimeDelta));
	//}
	//
	//if (GetAsyncKeyState('D'))
	//{
	//	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(180.f * fTimeDelta));
	//}

}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain* pTerrain = new CTerrain(pGraphicDev);

	if (FAILED(pTerrain->Ready_GameObject()))
	{
		Safe_Release(pTerrain);
		MSG_BOX("pTerrain Create Failed");
		return nullptr;
	}

	return pTerrain;
}

void CTerrain::Free()
{
	Engine::CGameObject::Free();
}
