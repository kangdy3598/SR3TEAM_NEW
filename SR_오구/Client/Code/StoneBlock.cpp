#include "pch.h"
#include "StoneBlock.h"
#include "Export_Utility.h"

CStoneBlock::CStoneBlock(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_bIsMove(false), m_bIsUp(true), m_fMoveTime(0), m_iHoleImageID(0), m_iImageID(0), m_bActiceOnce(false)
{
}

CStoneBlock::~CStoneBlock()
{
}

HRESULT CStoneBlock::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	SetObjectType(OBJ_TYPE::NOTPASS_ABLE);
	m_pTransformCom->m_vScale = { 14.f, 14.f, 14.f };	
	m_vecTexture.resize(7);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_StoneBlock.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_StoneBlockOnce.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_StoneBlockOnce_Clear.png", &m_vecTexture[2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_OceanStoneBlockOnce.png", &m_vecTexture[3]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_OceanStoneBlockOnce_Clear.png", &m_vecTexture[4]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_StoneBlockOnce_Sun.png", &m_vecTexture[5]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_StoneBlockOnce_Sun_Clear.png", &m_vecTexture[6]);

	m_pHoleTransformCom->m_vScale = { 16.f, 16.f, 0.f };
	m_pHoleTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);
	m_vecHoleTexture.resize(2);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_StoneBlock_Hole.png", &m_vecHoleTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_StoneBlock_Down.png", &m_vecHoleTexture[1]);
	return S_OK;
}

_int CStoneBlock::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (m_bIsMove && m_fMoveTime < 1.f) {

		m_fMoveTime += fTimeDelta;
		_vec3 vPos, vMovePos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_fTargetPos.x = vPos.x;
		m_fTargetPos.z = vPos.z;
		vMovePos = m_fTargetPos - vPos;

		m_pTransformCom->Move_Pos(&vMovePos, fTimeDelta, 4.f);

		if (m_fMoveTime > 0.4f) {			
			m_iHoleImageID = m_bIsUp? 0 : 1;
		}

		if (m_fMoveTime >= 0.9f) {
			m_bIsMove = false;
		}
	}

	return iExit;
}

void CStoneBlock::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CStoneBlock::Render_GameObject()
{	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pHoleTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecHoleTexture[m_iHoleImageID]);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CStoneBlock::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pHoleTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_HoleTransform", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

void CStoneBlock::Init(_float _fX, _float _fZ, _bool _bIsUp)
{		
	_float fTempY;	
	fTempY = _bIsUp ? 13.5f : -14.f;
	m_iHoleImageID = _bIsUp ? 0 : 1;
	m_pTransformCom->Set_Pos(_fX, fTempY, _fZ);
	m_pHoleTransformCom->Set_Pos(_fX, 0.2f, _fZ + 10.5f);
	m_bIsUp = _bIsUp;
}

CStoneBlock* CStoneBlock::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStoneBlock* pStoneBlock = new CStoneBlock(pGraphicDev);

	if (FAILED(pStoneBlock->Ready_GameObject()))
	{
		Safe_Release(pStoneBlock);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pStoneBlock);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PUZZLE, pStoneBlock);
	return pStoneBlock;
}

void CStoneBlock::Move_StoneBlock()
{
	m_bIsMove = true;
	m_bIsUp = m_bIsUp == true ? false : true;
	m_fTargetPos = m_bIsUp == true ? _vec3{ 0.f, 13.5f, 0.f } : _vec3{ 0.f, -14.5f, 0.f };
	m_iHoleImageID = 0;
	m_fMoveTime = 0;
}

void CStoneBlock::Move_StoneBlockOnce()
{
	if (m_bActiceOnce)
		return;

	m_bActiceOnce = true;
	m_bIsMove = true;
	m_bIsUp = false;
	m_fTargetPos = _vec3{ 0.f, -14.5f, 0.f };
	m_iHoleImageID = 0;
	m_fMoveTime = 0;

	Play_Sound(L"SFX_168_GateOnceOff.wav", SOUND_EFFECT, 1.f);
}

void CStoneBlock::Free()
{
	Engine::CGameObject::Free();
}
