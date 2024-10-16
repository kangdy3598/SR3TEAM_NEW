#include "pch.h"
#include "ColorStone.h"
#include "Export_Utility.h"

CColorStone::CColorStone(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_iImageID(1), m_pComStone(nullptr), m_bIsLift(false)
{
}

CColorStone::~CColorStone()
{
}

HRESULT CColorStone::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 16.f, 16.f, 16.f };	
	int type = OBJ_TYPE::LIFT_ABLE + OBJ_TYPE::PUSH_ABLE;
	SetObjectType(type); 
	m_eTag = TAG_STONE; 
	m_vecTexture.resize(6); 
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_JellyPushableCyan.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_JellyPushableYellow.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_JellyPushableMagenta.png", &m_vecTexture[2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_JellyPushableGreen.png", &m_vecTexture[3]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_JellyPushableBlue.png", &m_vecTexture[4]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_JellyPushableRed.png", &m_vecTexture[5]);	

	return S_OK;
}

_int CColorStone::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsActive)
		return 0;

	Add_RenderGroup(RENDER_ALPHA, this);
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CColorStone::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsActive)
		return;

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CColorStone::Render_GameObject()
{
	if (!m_bIsActive)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();
	////m_pBoundBox->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CColorStone::OnCollision(CGameObject* _pOther)
{
	if (Engine::GetKeyDown(DIK_S))
	{
		if (_pOther->GetObjectKey() != L"PlayerInteractionBox")
			return;

		if (m_pComStone == nullptr) {
			Play_Sound(L"SFX_47_Jelly_Seperate.wav", SOUND_EFFECT, 1.f);
			return;
		}			

		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_bIsLift = true;	
		m_iImageID = m_iImageID - static_cast<CColorStone*>(m_pComStone)->Get_ImageID() - 2;
		m_pTransformCom->m_vScale = { 16.f, 16.f, 16.f };
		static_cast<Engine::CTransform*>(m_pComStone->Get_Component(ID_DYNAMIC, L"Com_Transform"))->Set_Pos(vPos.x, 14.f, vPos.z + 15.f);
		m_pComStone->Set_Active(true);
		m_pComStone = nullptr;

		Play_Sound(L"SFX_47_Jelly_Seperate.wav", SOUND_EFFECT, 1.f);
	}
}

void CColorStone::OnCollisionEnter(CGameObject* _pOther)
{	
	if (!m_bIsActive)
		return;

	if (_pOther->Get_Tag() == TAG_STONE) {
		if (static_cast<CColorStone*>(_pOther)->Get_ImageID() == m_iImageID ||
			static_cast<CColorStone*>(_pOther)->Is_Combined() || static_cast<CColorStone*>(_pOther)->Is_Lifted())
			return;

		if (!_pOther->Is_Active() || m_pComStone != nullptr || m_bIsLift)
			return;

		m_iImageID = m_iImageID + static_cast<CColorStone*>(_pOther)->Get_ImageID() + 2;
		m_pTransformCom->m_vScale = { 20.f, 20.f, 20.f };
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vPos.y = 20.f;
		m_pTransformCom->Set_Pos(vPos);

		_pOther->Set_Active(false);
		m_pComStone = _pOther;

		Play_Sound(L"SFX_45_Jelly_Combine.wav", SOUND_EFFECT, 1.f);
	}
}

void CColorStone::OnCollisionExit(CGameObject* _pOther)
{
	if (_pOther->GetObjectKey() != L"PlayerInteractionBox")
		return;

	if (m_bIsLift) {
		m_bIsLift = false;
		return;
	}
}

HRESULT CColorStone::Add_Component()
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

CColorStone* CColorStone::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CColorStone* pCrystal = new CColorStone(pGraphicDev);

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

void CColorStone::Free()
{
	Engine::CGameObject::Free();
}