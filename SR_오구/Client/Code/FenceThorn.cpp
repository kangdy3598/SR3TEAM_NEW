#include "pch.h"
#include "FenceThorn.h"
#include "Player.h"
#include "Export_Utility.h"

CFenceThorn::CFenceThorn(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_iImageID(0)
{
}

CFenceThorn::~CFenceThorn()
{
}

HRESULT CFenceThorn::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//int type = OBJ_TYPE::NOTPASS_ABLE + OBJ_TYPE::HURT_ABLE;
	SetObjectType(NOTPASS_ABLE);
	m_vecTexture.resize(2);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_FenceThornVertical.png", &m_vecTexture[0]);
	LoadTextureFromFile(m_pGraphicDev, "../Bin/Resource/Texture/puzzle/Sprite_MoonForest_FenceThornHorizontal.png", &m_vecTexture[1]);

	return S_OK;
}

_int CFenceThorn::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CFenceThorn::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CFenceThorn::Render_GameObject()
{
	if (CManagement::GetInstance()->m_imap_stage == 2)
	{
		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTexTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_vecTexture[m_iImageID]);
	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	////m_pBoundBox->Render_Buffer();
	if (CManagement::GetInstance()->m_imap_stage == 2)
	{
		m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, false);
	}
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CFenceThorn::Add_Component()
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

void CFenceThorn::Init(_bool _bHorizontal, _float _fX, _float _fZ)
{
	if (_bHorizontal) {
		m_pTransformCom->m_vScale = { 32.f, 16.f, 16.f };
		m_pTexTransformCom->m_vScale = { 32.f, 16.f, 0.f };		
		m_iImageID = 1;
	}
	else {
		m_pTransformCom->m_vScale = { 16.f, 16.f, 32.f };
		m_pTexTransformCom->m_vScale = { 16.f, 32.f, 0.f };
	}

	m_pTransformCom->Set_Pos(_fX, 10.f, _fZ);
	m_pTexTransformCom->Set_Pos(_fX, 10.f, _fZ);
	m_pTexTransformCom->Rotation(ROT_X, 90.f * 3.14159265359f / 180.f);
}

void CFenceThorn::OnCollision(CGameObject* _pOther)
{
}

void CFenceThorn::OnCollisionEnter(CGameObject* _pOther)
{
	/*if (_pOther->Get_Tag() == TAG_PLAYER) {
		static_cast<CPlayer*>(_pOther)->SetPlayerCurHP(-1);
	}*/

}

void CFenceThorn::OnCollisionExit(CGameObject* _pOther)
{
	
}

CFenceThorn* CFenceThorn::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFenceThorn* pPressBlock = new CFenceThorn(pGraphicDev);

	if (FAILED(pPressBlock->Ready_GameObject()))
	{
		Safe_Release(pPressBlock);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pPressBlock);
	return pPressBlock;
}

void CFenceThorn::Free()
{
	Engine::CGameObject::Free();
}