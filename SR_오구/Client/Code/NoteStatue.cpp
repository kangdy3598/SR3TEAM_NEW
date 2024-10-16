#include "pch.h"
#include "NoteStatue.h"
#include "MusicStatue.h"
#include "Export_Utility.h"

CNoteStatue::CNoteStatue(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_fActiveTime(0)
{
}

CNoteStatue::~CNoteStatue()
{
}

HRESULT CNoteStatue::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	SetObjectType(NOTPASS_ABLE);
	m_pTransformCom->m_vScale = { 16.f, 20.f, 16.f };
	return S_OK;
}

_int CNoteStatue::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);	
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (m_bIsActivate) {
		m_fActiveTime += fTimeDelta;

		if (m_fActiveTime >= 1.5f)
		{
			m_bIsActivate = false;
			m_fActiveTime = 0;
		}
	}
	return iExit;
}

void CNoteStatue::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CNoteStatue::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CNoteStatue::OnCollision(CGameObject* _pOther)
{
	if (_pOther->GetObjectKey() != L"PlayerInteractionBox")
		return;

	m_CPlayer = dynamic_cast<CPlayerInteractionBox*>(_pOther)->GetPlayer();
	if (m_CPlayer->GetSwingTrigger() && !m_bIsActivate)
	{
		Play_Note();
		m_bIsActivate = true;
	}
}

void CNoteStatue::OnCollisionEnter(CGameObject* _pOther)
{

}

void CNoteStatue::OnCollisionExit(CGameObject* _pOther)
{
	if (_pOther->GetObjectKey() != L"PlayerInteractionBox")
		return;

	m_bIsActivate = false;
}

void CNoteStatue::Play_Note()
{
	switch (m_iNoteID)
	{
	case 0:
		Play_Sound(L"SFX_674_MusicIceDrum1.wav", SOUND_EFFECT, 1.f);
		break;
	case 1:
		Play_Sound(L"SFX_675_MusicIceDrum2.wav", SOUND_EFFECT, 1.f);
		break;
	case 2:
		Play_Sound(L"SFX_676_MusicIceDrum3.wav", SOUND_EFFECT, 1.f);
		break;
	case 3:
		Play_Sound(L"SFX_677_MusicIceDrum4.wav", SOUND_EFFECT, 1.f);
		break;
	case 4:
		Play_Sound(L"SFX_678_MusicIceDrum5.wav", SOUND_EFFECT, 1.f);
		break;
	case 5:
		Play_Sound(L"SFX_679_MusicIceDrum6.wav", SOUND_EFFECT, 1.f);
		break;
	default:
		break;
	}
	static_cast<CMusicStatue*>(m_pGroup)->Match_Note(m_iNoteID);
}

HRESULT CNoteStatue::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_NoteStatue"));
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

CNoteStatue* CNoteStatue::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNoteStatue* pNoteStatue = new CNoteStatue(pGraphicDev);

	if (FAILED(pNoteStatue->Ready_GameObject()))
	{
		Safe_Release(pNoteStatue);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pNoteStatue);
	return pNoteStatue;
}

void CNoteStatue::Free()
{
	Engine::CGameObject::Free();
}
