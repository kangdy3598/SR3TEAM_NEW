#include "pch.h"
#include "FirePit.h"
#include "Export_Utility.h"

CFirePit::CFirePit(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_bIsIgnite(false), m_iImageID(0), m_iTargetID(8), m_fTime(0)
{
}

CFirePit::~CFirePit()
{
}

HRESULT CFirePit::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 16.f, 20.f, 16.f };
	m_pFireTransformCom->m_vScale = { 20.f, 20.f, 16.f };
	SetObjectType(NOTPASS_ABLE);
	m_iImageID = 0;
	m_vecTexture.resize(8);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/FirePit_Anim/Sprite_MoonBossHammerJumpEffect_01.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/FirePit_Anim/Sprite_MoonBossHammerJumpEffect_02.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/FirePit_Anim/Sprite_MoonBossHammerJumpEffect_03.png", &m_vecTexture[2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/FirePit_Anim/Sprite_MoonBossHammerJumpEffect_04.png", &m_vecTexture[3]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/FirePit_Anim/Sprite_MoonBossHammerJumpEffect_05.png", &m_vecTexture[4]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/FirePit_Anim/Sprite_MoonBossHammerJumpEffect_06.png", &m_vecTexture[5]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/FirePit_Anim/Sprite_MoonBossHammerJumpEffect_07.png", &m_vecTexture[6]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/FirePit_Anim/Sprite_MoonBossHammerJumpEffect_08.png", &m_vecTexture[7]);	

	return S_OK;
}

_int CFirePit::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);

	if (m_bIsIgnite && m_iImageID != m_iTargetID) {
		m_fTime += fTimeDelta;

		if (m_fTime >= 0.02f) {
			if (m_iTargetID > m_iImageID)
				m_iImageID++;
			
			if (m_iImageID == m_iTargetID)
				m_iImageID = 0;

			m_fTime = 0;
		}
	}


	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CFirePit::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CFirePit::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	if (m_bIsIgnite) {
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pFireTransformCom->Get_WorldMatrix());
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CFirePit::Init_Pos(_float _fX, _float _fZ)
{
	m_pTransformCom->Set_Pos(_fX, 20.f, _fZ);
	m_pFireTransformCom->Set_Pos(_fX, 35.f, _fZ);
}


HRESULT CFirePit::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_FirePit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pFireTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_FireTransform", pComponent });

	return S_OK;
}

CFirePit* CFirePit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFirePit* pMusicStatue = new CFirePit(pGraphicDev);

	if (FAILED(pMusicStatue->Ready_GameObject()))
	{
		Safe_Release(pMusicStatue);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	return pMusicStatue;
}

void CFirePit::Free()
{
	Engine::CGameObject::Free();
}
