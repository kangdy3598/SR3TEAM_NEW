#include "pch.h"
#include "StoneHole.h"
#include "Export_Utility.h"

CStoneHole::CStoneHole(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_pStoneBlock(nullptr), m_iImageID(0), m_pCollided(nullptr), m_fPullDuration(0)
{
}

CStoneHole::~CStoneHole()
{
}

HRESULT CStoneHole::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 15.f, 20.f, 15.f };
	m_pTexTransformCom->m_vScale = { 18.f, 18.f, 18.f };
	m_pTexTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);
	SetObjectType(NOTPASS_ABLE);
	m_vecTexture.resize(2);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_StoneHole.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_StoneHoleFill.png", &m_vecTexture[1]);

	return S_OK;
}

_int CStoneHole::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (m_pCollided != nullptr && m_fPullDuration < .5f) {
		m_fPullDuration += fTimeDelta;

		_vec3 vPos, vMovePos, vTarget;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		Engine::CTransform* StoneTrasnform = static_cast<Engine::CTransform*>(m_pCollided->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		StoneTrasnform->Get_Info(INFO_POS, &vTarget);
		vPos.z -= 10.f;
		vMovePos = vPos - vTarget;
		vMovePos.y = 0.f;
		StoneTrasnform->Move_Pos(&vMovePos, fTimeDelta, 4.f);

		if (m_fPullDuration >= .5f) {
			m_pCollided->Set_Active(false);
			m_bIsActive = false;
			m_iImageID = 1;			
			Play_Sound(L"SFX_43_StonePushable_Fall.wav", SOUND_EFFECT, 1.f);
		}
	}

	return iExit;
}

void CStoneHole::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CStoneHole::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTexTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CStoneHole::OnCollision(CGameObject* _pOther)
{
}

void CStoneHole::OnCollisionEnter(CGameObject* _pOther)
{
	if (_pOther->Get_Tag() == TAG_STONE) {
		if (m_pCollided != nullptr)
			return;

		m_pCollided = _pOther;
		m_bIsActive = false;
	}
}

void CStoneHole::OnCollisionExit(CGameObject* _pOther)
{
}

HRESULT CStoneHole::Add_Component()
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

	pComponent = m_pTexTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TexTransform", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

CStoneHole* CStoneHole::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStoneHole* pStoneBlockHole = new CStoneHole(pGraphicDev);

	if (FAILED(pStoneBlockHole->Ready_GameObject()))
	{
		Safe_Release(pStoneBlockHole);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pStoneBlockHole);
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::PUZZLE, pStoneBlockHole);
	return pStoneBlockHole;
}

void CStoneHole::Free()
{
	Engine::CGameObject::Free();
}
