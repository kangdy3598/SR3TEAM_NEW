#include "pch.h"
#include "BoardCursor.h"
#include "Export_Utility.h"
#include "Export_System.h"

CBoardCursor::CBoardCursor(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_bIsActive(false)
{

}

CBoardCursor::~CBoardCursor()
{
}

HRESULT CBoardCursor::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 15.f, 15.f, 0.f };
	m_pTransformCom->Rotation(ROT_X, 90.f * 3.14f / 180.f);
	//m_pTransformCom->Rotation(ROT_Y, 90.f * 3.14f / 180.f);
	m_pTransformCom->Set_Pos(0.f, 0.2f, -12.2f);
	return S_OK;
}

_int CBoardCursor::Update_GameObject(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (!m_bIsActive)
		return iExit;

	Add_RenderGroup(RENDER_TRANSLUCNET, this);

	//Engine::CTransform* pPlayerTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(ID_DYNAMIC, L"Layer_GameLogic", L"Player", L"Com_Transform"));
	//NULL_CHECK_RETURN(pPlayerTransform, -1);	
	//_vec3   vPlayerPos;
	//pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);

	//m_pTransformCom->Chase_Target(&vPlayerPos, 5.f * fTimeDelta);

	return iExit;
}

void CBoardCursor::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsActive)
		return;

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CBoardCursor::Render_GameObject()
{
	if (!m_bIsActive)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBoardCursor::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PipeCursorTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

CBoardCursor* CBoardCursor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoardCursor* pCursor = new CBoardCursor(pGraphicDev);

	if (FAILED(pCursor->Ready_GameObject()))
	{
		Safe_Release(pCursor);
		MSG_BOX("pPipeBoard Create Failed");
		return nullptr;
	}

	return pCursor;
}
void CBoardCursor::Key_Input(const _float& fTimeDelta)
{

}
void CBoardCursor::Free()
{
}
