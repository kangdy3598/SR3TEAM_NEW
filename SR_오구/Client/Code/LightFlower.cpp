#include "pch.h"
#include "LightFlower.h"
#include "FlowerGlow.h"
#include "ColorStone.h"
#include "StonePedestal.h"
#include "Export_Utility.h"

CLightFlower::CLightFlower(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CPuzzleObject(pGraphicDev), m_iImageID(0), m_iTargetID(0), m_fTime(0)
{
}

CLightFlower::~CLightFlower()
{
}

HRESULT CLightFlower::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 37.f, 25.f, 0.f };	
	m_pRootTransformCom->m_vScale = { 20.f, 30.f, 0.f };
	m_vecTexture.resize(6);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/LightFlower_Anim/Sprite_MoonForest_FlowerLight_03_09.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/LightFlower_Anim/Sprite_MoonForest_FlowerLight_03_10.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/LightFlower_Anim/Sprite_MoonForest_FlowerLight_03_11.png", &m_vecTexture[2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/LightFlower_Anim/Sprite_MoonForest_FlowerLight_03_13.png", &m_vecTexture[3]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/LightFlower_Anim/Sprite_MoonForest_FlowerLight_03_14.png", &m_vecTexture[4]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/LightFlower_Anim/Sprite_MoonForest_FlowerLight_03_15.png", &m_vecTexture[5]);

	return S_OK;
}

_int CLightFlower::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (m_iImageID != m_iTargetID) {
		m_fTime += fTimeDelta;

		if (m_fTime >= 0.07f) {
			if (m_iTargetID < m_iImageID)
				m_iImageID--;
			else if (m_iTargetID > m_iImageID)
				m_iImageID++;

			m_fTime = 0;
		}
	}

	if (m_pGlow != nullptr)
		m_pGlow->Update_GameObject(fTimeDelta);

	return iExit;
}

void CLightFlower::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);


	if (m_pGlow != nullptr)
		m_pGlow->LateUpdate_GameObject(fTimeDelta);
}

void CLightFlower::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();	

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pRootTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	if (m_pGlow != nullptr)
		m_pGlow->Render_GameObject();
}

HRESULT CLightFlower::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LightFlower"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pRootTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_RootTransform", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

void CLightFlower::Init_Pos(_float _fX, _float _fZ)
{
	m_pTransformCom->Set_Pos(_fX -2.f, 65.f, _fZ);
	m_pRootTransformCom->Set_Pos(_fX, 30.f, _fZ);
}

void CLightFlower::Match_Puzzle()
{
	if (static_cast<CStonePedestal*>(m_pPedestal)->Get_Stone() == nullptr) {
		m_pGlow->Set_Active(false);
		m_iImageID = 0;
		m_iTargetID = 0;

		Play_Sound(L"SFX_79_FlowerLightStandOff.wav", SOUND_EFFECT, 1.f);
		return;
	}
		
	int iIndex = static_cast<CColorStone*>(static_cast<CStonePedestal*>(m_pPedestal)->Get_Stone())->Get_ImageID();
	if (iIndex >= 0) {
			
		m_iTargetID = 5;
		static_cast<CFlowerGlow*>(m_pGlow)->Set_ImageID(iIndex);
		m_pGlow->Set_Active(true);		

		Play_Sound(L"SFX_78_FlowerLightStandOn.wav", SOUND_EFFECT, 1.f);
	}		
}

CLightFlower* CLightFlower::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLightFlower* pCrystal = new CLightFlower(pGraphicDev);

	if (FAILED(pCrystal->Ready_GameObject()))
	{
		Safe_Release(pCrystal);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	return pCrystal;
}

void CLightFlower::Free()
{
	Engine::CGameObject::Free();
}
