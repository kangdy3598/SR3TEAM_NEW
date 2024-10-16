#include "pch.h"
#include "StartBackGround.h"
#include "Export_Utility.h"

CStartBackGround::CStartBackGround(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
{
}

CStartBackGround::~CStartBackGround()
{
}

HRESULT CStartBackGround::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CStartBackGround::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);	

	if(GetAsyncKeyState)

	return iExit;
}

void CStartBackGround::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CStartBackGround::Render_GameObject()
{
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CStartBackGround::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex_Test"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });


	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_StartBackGroundTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CStartBackGround* CStartBackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStartBackGround* pStartBackGround = new CStartBackGround(pGraphicDev);

	if(FAILED(pStartBackGround->Ready_GameObject()))
	{
		Safe_Release(pStartBackGround);
		MSG_BOX("pStartBackGround Create Failed");
		return nullptr;
	}

	return pStartBackGround;
}

void CStartBackGround::Free()
{
	Engine::CGameObject::Free();
}
