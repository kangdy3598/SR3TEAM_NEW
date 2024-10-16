#include "pch.h"
#include "MusicStatue.h"
#include "FirePit.h"
#include "StoneBlock.h"
#include "Export_Utility.h"

CMusicStatue::CMusicStatue(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_iCurNote(0), m_bIsClear(false), m_fActiveTime(0)
{
}

CMusicStatue::~CMusicStatue()
{
}

HRESULT CMusicStatue::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	SetObjectType(NOTPASS_ABLE);
	m_pTransformCom->m_vScale = { 17.f, 16.f, 15.f };
	m_vecKeyOrder.push_back(2);
	m_vecKeyOrder.push_back(1);
	m_vecKeyOrder.push_back(3);
	m_vecKeyOrder.push_back(0);
	m_vecKeyOrder.push_back(5);
	m_vecKeyOrder.push_back(4);
	return S_OK;
}

_int CMusicStatue::Update_GameObject(const _float& fTimeDelta)
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

void CMusicStatue::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);	
}

void CMusicStatue::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CMusicStatue::OnCollision(CGameObject* _pOther)
{
	if (_pOther->GetObjectKey() != L"PlayerInteractionBox")
		return;

	m_CPlayer = dynamic_cast<CPlayerInteractionBox*>(_pOther)->GetPlayer();
	if (m_CPlayer->GetSwingTrigger() && !m_bIsActivate)
	{
		Play_Music();
		m_bIsActivate = true;
	}
}

void CMusicStatue::OnCollisionEnter(CGameObject* _pOther)
{
}

void CMusicStatue::OnCollisionExit(CGameObject* _pOther)
{
	if (_pOther->GetObjectKey() != L"PlayerInteractionBox")
		return;

	m_bIsActivate = false;
}

void CMusicStatue::Play_Music()
{
	Play_Sound(L"SFX_676_MusicIceDrum_join.wav", SOUND_EFFECT, 1.f);
}

void CMusicStatue::Match_Note(_int _iNote)
{
	if (m_bIsClear)
		return;

	if (m_vecKeyOrder[m_iCurNote] != _iNote) {
		Reset_FirePit();
		return;
	}

	static_cast<CFirePit*>(m_vecFirePits[m_iCurNote])->Set_Ignite(true);
	++m_iCurNote;
	//Play_Sound(L"SFX_360_FireTorchLit.wav", SOUND_SURROUNDING, 1.f);	

	if (m_iCurNote >= m_vecFirePits.size()) {
		Clear_Puzzle();
		return;
	}
}

void CMusicStatue::Reset_FirePit()
{
	for (auto& iter : m_vecFirePits)
		static_cast<CFirePit*>(iter)->Set_Ignite(false);

	m_iCurNote = 0;
}

void CMusicStatue::Clear_Puzzle()
{
	for (int i = 0; i < m_vecStoneBlocks.size(); ++i)
	{
		static_cast<CStoneBlock*>(m_vecStoneBlocks[i])->Set_ImageID(4);
		static_cast<CStoneBlock*>(m_vecStoneBlocks[i])->Move_StoneBlock();
	}

	m_bIsClear = true;
}

HRESULT CMusicStatue::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MusicStatue"));
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

CMusicStatue* CMusicStatue::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMusicStatue* pMusicStatue = new CMusicStatue(pGraphicDev);

	if (FAILED(pMusicStatue->Ready_GameObject()))
	{
		Safe_Release(pMusicStatue);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pMusicStatue);
	return pMusicStatue;
}

void CMusicStatue::Free()
{
	Engine::CGameObject::Free();
}
