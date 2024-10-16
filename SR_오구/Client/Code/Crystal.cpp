#include "pch.h"
#include "Crystal.h"
#include "Export_Utility.h"

CCrystal::CCrystal(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_iImageID(1), m_fTime(0), m_iTargetID(1)
{
}

CCrystal::~CCrystal()
{
}

HRESULT CCrystal::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 18.f, 18.f, 0.f };
	SetObjectType(OBJ_TYPE::NOTPASS_ABLE);
	m_vecTexture.resize(11);	
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_CrystalPuzzle_Gray.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_CrystalPuzzle_Yellow.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_CrystalPuzzle_Green.png", &m_vecTexture[2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_CrystalPuzzle_Red.png", &m_vecTexture[3]);	

	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Crystal_Puzzle_Anim/Sprite_CrystalPuzzle_01.png", &m_vecTexture[4]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Crystal_Puzzle_Anim/Sprite_CrystalPuzzle_02.png", &m_vecTexture[5]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Crystal_Puzzle_Anim/Sprite_CrystalPuzzle_03.png", &m_vecTexture[6]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Crystal_Puzzle_Anim/Sprite_CrystalPuzzle_04.png", &m_vecTexture[7]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Crystal_Puzzle_Anim/Sprite_CrystalPuzzle_05.png", &m_vecTexture[8]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Crystal_Puzzle_Anim/Sprite_CrystalPuzzle_06.png", &m_vecTexture[9]);	

	return S_OK;
}

_int CCrystal::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsActive)
		return 0;
		
	Add_RenderGroup(RENDER_ALPHA, this);


	if (m_iImageID != m_iTargetID) {
		m_fTime += fTimeDelta;

		if (m_fTime >= 0.04f) {
			if (m_iTargetID > m_iImageID)
				m_iImageID++;

			if (m_iImageID == m_iTargetID)
				m_bIsActive = false;

			m_fTime = 0;
		}
	}


	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CCrystal::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsActive)
		return;

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CCrystal::Render_GameObject()
{
	if (!m_bIsActive)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);	
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CCrystal::Break()
{
	m_iImageID = 4;
	m_iTargetID = 9;
}

HRESULT CCrystal::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PipeTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
	
	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

CCrystal* CCrystal::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCrystal* pCrystal = new CCrystal(pGraphicDev);

	if (FAILED(pCrystal->Ready_GameObject()))
	{
		Safe_Release(pCrystal);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pCrystal);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PUZZLE, pCrystal);
	return pCrystal;
}

void CCrystal::Free()
{
	Engine::CGameObject::Free();
}