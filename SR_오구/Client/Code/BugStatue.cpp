#include "pch.h"
#include "BugStatue.h"
#include "BugStatuePuzzle.h"
#include "PuzzleObject.h"
#include "Export_Utility.h"

CBugStatue::CBugStatue(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_bIsActivate(false)
{
}

CBugStatue::~CBugStatue()
{
}

HRESULT CBugStatue::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 16.f, 20.f, 16.f };
	SetObjectType(OBJ_TYPE::NOTPASS_ABLE);
	m_iImageID = 0; 
	m_vecTexture.resize(4); 
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_BugStatueOnce_Glow.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_BugStatueOnce.png", &m_vecTexture[1]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MonkeyStatue_Cave_Once.png", &m_vecTexture[2]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MonkeyStatue_Cave.png", &m_vecTexture[3]);

	return S_OK;
}

_int CBugStatue::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);	
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CBugStatue::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CBugStatue::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBugStatue::OnCollision(CGameObject* _pOther)
{
	if (_pOther->GetObjectKey() != L"PlayerInteractionBox")
		return;

	m_CPlayer = dynamic_cast<CPlayerInteractionBox*>(_pOther)->GetPlayer();
	if (m_CPlayer->GetSwingTrigger() && !m_bIsActivate)
	{
		Active_StoneBlock();
		m_bIsActivate = true;
		Play_Sound(L"SFX_23_StoneGateLightOn.wav", SOUND_EFFECT, 1.f);
	}
}

void CBugStatue::OnCollisionEnter(CGameObject* _pOther)
{
}

void CBugStatue::OnCollisionExit(CGameObject* _pOther)
{
}

void CBugStatue::Active_StoneBlock()
{
	m_iImageID++;
	static_cast<CPuzzleObject*>(m_pGroup)->Match_Puzzle();
}

HRESULT CBugStatue::Add_Component()
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

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

CBugStatue* CBugStatue::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBugStatue* pBugStatue = new CBugStatue(pGraphicDev);

	if (FAILED(pBugStatue->Ready_GameObject()))
	{
		Safe_Release(pBugStatue);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pBugStatue);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PUZZLE, pBugStatue);
	return pBugStatue;
}

void CBugStatue::Free()
{
	Engine::CGameObject::Free();
}
