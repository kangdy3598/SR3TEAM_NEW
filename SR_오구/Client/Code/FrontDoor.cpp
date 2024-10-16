#include "pch.h"
#include "FrontDoor.h"
#include "Export_Utility.h"

CFrontDoor::CFrontDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CPuzzleObject(pGraphicDev), m_bIsOpened(false), m_fOpenTime(0)
{
}

CFrontDoor::~CFrontDoor()
{
}

HRESULT CFrontDoor::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);	
	m_fDoorSize = 107.f;
	m_pLTransformCom->m_vScale = { 107.f, 107.f, 0.f };
	m_pRTransformCom->m_vScale = { 107.f, 107.f, 0.f };

	return S_OK;
}

_int CFrontDoor::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

			
	m_fOpenTime += fTimeDelta;	
	
	if (m_bIsOpened && m_fOpenTime <= 3.f) {
		m_pLTransformCom->Rotation(ROT_Y, .01f);
		m_pRTransformCom->Rotation(ROT_Y, -.01f);
	}
	else
		m_bIsOpened = false;

	return iExit;
}

void CFrontDoor::LateUpdate_GameObject(const _float& fTimeDeltsa)
{
}

void CFrontDoor::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pLTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pLTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pRTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pRTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
		
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CFrontDoor* CFrontDoor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFrontDoor* pCrystal = new CFrontDoor(pGraphicDev);

	if (FAILED(pCrystal->Ready_GameObject()))
	{
		Safe_Release(pCrystal);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}


	return pCrystal;
}

void CFrontDoor::Init_Pos(_float _fX, _float _fZ)
{
	m_pLTransformCom->Set_Pos(_fX - 107.f, 107.f, _fZ);
	m_pRTransformCom->Set_Pos(_fX + 107.f, 107.f, _fZ);
}

void CFrontDoor::Match_Puzzle()
{
	m_bIsOpened = true;
	m_fOpenTime = 0;
}


HRESULT CFrontDoor::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pLTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_FrontDoorLeft"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_LTexture", pComponent });

	pComponent = m_pRTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_FrontDoorRight"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_RTexture", pComponent });

	pComponent = m_pLTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_LTransform", pComponent });


	pComponent = m_pRTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_RTransform", pComponent });

	return S_OK;
}

void CFrontDoor::Free()
{
	Engine::CGameObject::Free();
}

