#include "pch.h"
#include "Trigger.h"
#include "PuzzleObject.h"
#include "Export_Utility.h"

CTrigger::CTrigger(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CTrigger::~CTrigger()
{
}

HRESULT CTrigger::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	return S_OK;
}

_int CTrigger::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CTrigger::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTrigger::Render_GameObject()
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	////m_pBoundBox->Render_Buffer();
}

void CTrigger::OnCollisionEnter(CGameObject* _pOther)
{
	if (_pOther->Get_Tag() == TAG_PLAYER) {
		static_cast<CPuzzleObject*>(m_pGroup)->Match_Puzzle();
		m_bIsActive = false;
	}		
}

HRESULT CTrigger::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	return S_OK;
}

CTrigger* CTrigger::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrigger* pCrystal = new CTrigger(pGraphicDev);

	if (FAILED(pCrystal->Ready_GameObject()))
	{
		Safe_Release(pCrystal);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}
	CManagement::GetInstance()->GetCurScenePtr()->Add_ObjectGroup(GROUP_TYPE::OBJECT, pCrystal);

	return pCrystal;
}

void CTrigger::Initialize(_float _fX, _float _fZ, _vec3 _vScale)
{	
	m_pTransformCom->m_vScale = { _vScale.x, _vScale.y, _vScale.z };
	m_pTransformCom->Set_Pos(_fX, _vScale.y, _fZ);
}

void CTrigger::Free()
{
	Engine::CGameObject::Free();
}