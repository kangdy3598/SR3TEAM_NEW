#include "pch.h"
#include "StoneBlockHole.h"
#include "Export_Utility.h"

CStoneBlockHole::CStoneBlockHole(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pStoneBlock(nullptr), m_fDuration(2)
{
}

CStoneBlockHole::~CStoneBlockHole()
{
}

HRESULT CStoneBlockHole::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 18.f, 18.f, 0.f };
	m_iImageID = 0;
	m_pTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);
	m_vecTexture.resize(2);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_StoneBlock_Hole.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_StoneBlock_Down.png", &m_vecTexture[1]);

	return S_OK;
}

_int CStoneBlockHole::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (m_fDuration < 0.5f) {
		m_fDuration += fTimeDelta;
		if (m_fDuration > 0.5f)
			m_iImageID = 1;
	}

	return iExit;
}

void CStoneBlockHole::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CStoneBlockHole::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CStoneBlockHole::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MonkeyStatue"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

CStoneBlockHole* CStoneBlockHole::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStoneBlockHole* pStoneBlockHole = new CStoneBlockHole(pGraphicDev);

	if (FAILED(pStoneBlockHole->Ready_GameObject()))
	{
		Safe_Release(pStoneBlockHole);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pStoneBlockHole);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PUZZLE, pStoneBlockHole);
	return pStoneBlockHole;
}

void CStoneBlockHole::Free()
{
	Engine::CGameObject::Free();
}
