#include "pch.h"
#include "B52Bomber.h"
#include "WhitePhosphorus.h"
#include "Player.h"
#include "Export_Utility.h"

CB52Bomber::CB52Bomber(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_fFireRate(4.04f), m_fTime(0), m_iFireCount(0), m_bIsCalled(false)
{
}

CB52Bomber::~CB52Bomber()
{
}

HRESULT CB52Bomber::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 300.f, 300.f, 0.f };
	m_bIsActive = false;
	m_pTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);
	m_pTransformCom->Rotation(ROT_Y, 90.f * 3.14159265359f / 180.f);
	return S_OK;
}

void CB52Bomber::LateReady_GameObject()
{
	m_CPlayer = static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
}

_int CB52Bomber::Update_GameObject(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	for (auto& iter : m_vecWhitePhosphorus)
		iter->Update_GameObject(fTimeDelta);

	if (!m_bIsActive)
		return 0;

	Add_RenderGroup(RENDER_TRANSLUCNET, this);
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	_vec3  vRight;	
	m_pTransformCom->Get_Info(INFO_POS, &vRight);
	m_pTransformCom->Set_Pos(vRight.x + 30.f, vRight.y, vRight.z);

	m_fTime += fTimeDelta;
	if (m_fFireRate < m_fTime && m_bIsCalled) {

		if (m_iFireCount >= 5) {
			m_bIsCalled = false;
			return 0;
		}

		for (_int i = 0; i < 5; i++) {
			Shot();
		}			

		m_fTime = 0.f;
		m_fFireRate = 0.05f;
		m_iFireCount++;
	}

	if (m_fTime > 1.f && !m_bIsCalled) {
		Play_Sound(L"BombExplosion.wav", SOUND_PUZZLE_EXTRA, 1.f);

		static_cast<CDynamicCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"DynamicCamera"))->ShakeMove(1.f);
		m_bIsActive = false;
	}

	return iExit;
}

void CB52Bomber::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CB52Bomber::Render_GameObject()
{
	for (auto& iter : m_vecWhitePhosphorus)
		iter->Render_GameObject();

	if (!m_bIsActive)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CB52Bomber* CB52Bomber::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CB52Bomber* pCombinationPuzzle = new CB52Bomber(pGraphicDev);

	if (FAILED(pCombinationPuzzle->Ready_GameObject()))
	{
		Safe_Release(pCombinationPuzzle);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	return pCombinationPuzzle;
}

HRESULT CB52Bomber::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_B52"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

void CB52Bomber::Key_Input(const _float& fTimeDelta)
{
	if (Engine::GetKeyDown(DIK_7))
	{	
		_vec3 vPos;
		static_cast<Engine::CTransform*>(m_CPlayer->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &vPos);		
		m_pTransformCom->Set_Pos(vPos.x - 1000.f, 10.f, vPos.z + 100.f);
		m_bIsActive = true;
		m_bIsCalled = true;
		m_iFireCount = 0;	
		m_fTime = 0.f;
		m_fFireRate = 1.6f;

		Play_Sound(L"B52_Sound.wav", SOUND_PUZZLE_EXTRA, 1.f);
	}
}

void CB52Bomber::Pre_CreateShell()
{
	for (_int i = 0; i < 30; i++) {
		CGameObject* pOb = CWhitePhosphorus::Create(m_pGraphicDev);
		m_vecWhitePhosphorus.push_back(pOb);
	}	
}

void CB52Bomber::Shot()
{
	_vec3 vPos;
	static_cast<Engine::CTransform*>(m_CPlayer->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &vPos);

	for (int i = 0; i < m_vecWhitePhosphorus.size(); ++i)
	{
		if (m_vecWhitePhosphorus[i]->Is_Active())
			continue;
	
		static_cast<CWhitePhosphorus*>(m_vecWhitePhosphorus[i])->Init_Pos(vPos.x - 320.f, vPos.z);
		static_cast<CWhitePhosphorus*>(m_vecWhitePhosphorus[i])->Set_Velocity(_vec3(rand() % 20 + 10, 20.1f, rand() % 20 - 10));
		static_cast<CWhitePhosphorus*>(m_vecWhitePhosphorus[i])->Launch();	
		return;
	}

	Create_Shell();
}

void CB52Bomber::Create_Shell()
{
	_vec3 vPos;
	static_cast<Engine::CTransform*>(m_CPlayer->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Get_Info(INFO_POS, &vPos);

	CGameObject* pOb = CWhitePhosphorus::Create(m_pGraphicDev);
	static_cast<CWhitePhosphorus*>(pOb)->Init_Pos(vPos.x - 320.f, vPos.z);
	static_cast<CWhitePhosphorus*>(pOb)->Set_Velocity(_vec3(rand() % 20 + 10, 20.1f, rand() % 20 - 10));
	static_cast<CWhitePhosphorus*>(pOb)->Launch();

	m_vecWhitePhosphorus.push_back(pOb);
}


void CB52Bomber::Free()
{
	for (auto& iter : m_vecWhitePhosphorus)
		iter->Free();

	Engine::CGameObject::Free();
}
