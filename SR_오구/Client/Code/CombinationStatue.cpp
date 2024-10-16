#include "pch.h"
#include "CombinationStatue.h"
#include "CombinationPuzzle.h"
#include "Export_Utility.h"

CCombinationStatue::CCombinationStatue(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_iImageID(0), m_fActiveTime(0)
{
}

CCombinationStatue::~CCombinationStatue()
{
}

HRESULT CCombinationStatue::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 16.f, 20.f, 16.f };
	SetObjectType(OBJ_TYPE::NOTPASS_ABLE);
	m_vecTexture.resize(3);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MonkeyStatue_Ear.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MonkeyStatue_Eye.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MonkeyStatue_Mouth.png", &m_vecTexture[2]);

	return S_OK;
}

_int CCombinationStatue::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);

	if (m_bIsActivate) {
		m_fActiveTime += fTimeDelta;

		if (m_fActiveTime >= 1.f)
		{
			m_bIsActivate = false;
			m_fActiveTime = 0;
		}
	}

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CCombinationStatue::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CCombinationStatue::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CCombinationStatue::OnCollision(CGameObject* _pOther)
{
	if (_pOther->GetObjectKey() != L"PlayerInteractionBox")
		return;

	m_CPlayer = dynamic_cast<CPlayerInteractionBox*>(_pOther)->GetPlayer();
	if (m_CPlayer->GetSwingTrigger() && !m_bIsActivate)
	{
		m_bIsActivate = true;
		m_iImageID++;
		m_iImageID %= 3;
		Play_Sound(L"SFX_22_StoneGateMove.wav", SOUND_EFFECT, 1.f);
		static_cast<CCombinationPuzzle*>(m_pGroup)->Check_Matched();
	}
}

void CCombinationStatue::OnCollisionEnter(CGameObject* _pOther)
{
}

void CCombinationStatue::OnCollisionExit(CGameObject* _pOther)
{
	if (_pOther->GetObjectKey() != L"PlayerInteractionBox")
		return;

	m_bIsActivate = false;
}


HRESULT CCombinationStatue::Add_Component()
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

CCombinationStatue* CCombinationStatue::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCombinationStatue* pCombinationStatue = new CCombinationStatue(pGraphicDev);

	if (FAILED(pCombinationStatue->Ready_GameObject()))
	{
		Safe_Release(pCombinationStatue);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pCombinationStatue);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PUZZLE, pCombinationStatue);
	return pCombinationStatue;
}

void CCombinationStatue::Free()
{
	Engine::CGameObject::Free();
}
