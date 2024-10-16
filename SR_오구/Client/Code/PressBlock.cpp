#include "pch.h"
#include "PressBlock.h"
#include "CrystalPuzzle.h"
#include "Export_Utility.h"

CPressBlock::CPressBlock(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CPressBlock::~CPressBlock()
{
}

HRESULT CPressBlock::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 10.f, 10.f, 10.f };
	m_pTexTransformCom->m_vScale = { 15.f, 15.f, 0.f };
	m_pTexTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);
	m_iImageID = 0;
	m_bIsPressed = false;
	m_bIsCleared = false;
	m_pGroup = nullptr;

	m_vecTexture.resize(9);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_PressBlock_Up.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_PressBlock_Up_Yellow.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_PressBlock_Up_Green.png", &m_vecTexture[2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_PressBlock_Up_Red.png", &m_vecTexture[3]);

	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_PressBlock_Down.png", &m_vecTexture[4]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_PressBlock_Down_Yellow.png", &m_vecTexture[5]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_PressBlock_Down_Green.png", &m_vecTexture[6]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_PressBlock_Down_Red.png", &m_vecTexture[7]);

	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_PressBlock_Clear.png", &m_vecTexture[8]);

	return S_OK;
}

_int CPressBlock::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CPressBlock::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPressBlock::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTexTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if (m_bIsCleared)
		m_pGraphicDev->SetTexture(0, m_vecTexture[8]);
	else
		m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID + (m_bIsPressed * 4)]);
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPressBlock::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pTexTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TexTransform", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

void CPressBlock::Init(CGameObject* _pGroup, _int _iID, _float _fX, _float _fZ)
{
	m_pGroup = _pGroup;
	m_iMaxID = _iID + 2;
	m_pTransformCom->Set_Pos(_fX, 10.f, _fZ);
	m_pTexTransformCom->Set_Pos(_fX, 0.3f, _fZ);
}

void CPressBlock::OnCollision(CGameObject* _pOther)
{
}

void CPressBlock::OnCollisionEnter(CGameObject* _pOther)
{
	if (_pOther->Get_Tag() != TAG_PLAYER)
		return;

	if (m_bIsCleared) {
		m_iImageID = 8;
		m_bIsPressed = false;
		return;
	}

	m_iImageID++;
	m_iImageID %= m_iMaxID;
	m_bIsPressed = true;
	Play_Sound(L"SFX_27_StonePressBlock_On.wav", SOUND_EFFECT, 1.f);

	if (m_pGroup != nullptr)
		static_cast<CCrystalPuzzle*>(m_pGroup)->Check_Matched();
}

void CPressBlock::OnCollisionExit(CGameObject* _pOther)
{
	if (_pOther->Get_Tag() != TAG_PLAYER)
		return;

	m_bIsPressed = false;
	Play_Sound(L"SFX_28_StonePressBlock_Off.wav", SOUND_SURROUNDING, 1.f);
}

CPressBlock* CPressBlock::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPressBlock* pPressBlock = new CPressBlock(pGraphicDev);

	if (FAILED(pPressBlock->Ready_GameObject()))
	{
		Safe_Release(pPressBlock);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pPressBlock);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PUZZLE, pPressBlock);
	return pPressBlock;
}

void CPressBlock::Free()
{
	Engine::CGameObject::Free();
}