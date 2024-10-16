#include "pch.h"	
#include "BackGround.h"
#include "Export_Utility.h"


CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)

{
}

CBackGround::~CBackGround()
{
}

HRESULT CBackGround::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	return S_OK;
}

_int CBackGround::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return iExit;
}

void CBackGround::LateUpdate_GameObject(const _float& fTimeDelta)
{
	m_pAnimationCom->Update_Component_test(fTimeDelta,12,12,0,10,144);

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CBackGround::Render_GameObject()
{
	m_pTextureCom->Set_Texture();
	//m_pBufferCom->Render_Buffer(); // ¾¾¹ß ready_buffer ÇØ³ö¼­ ±×·¨´ø°Å¿´À½ ¾Æ¿À ¤»¤»
	m_pAnimationCom->Render_Buffer();
}

HRESULT CBackGround::Add_Component()
{
	CComponent* pComponent = NULL;

	//Proto_AnimationTex

	//pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });
	pComponent = m_pAnimationCom = dynamic_cast<CAnimationTex*>(Engine::Clone_Proto(L"Proto_AnimationTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Animation", pComponent });


	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LogoTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}


CBackGround* CBackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBackGround* pBackGround = new CBackGround(pGraphicDev);

	if(FAILED(pBackGround->Ready_GameObject()))
	{
		Safe_Release(pBackGround);
		MSG_BOX("pBackGround Create Failed");
		return nullptr;
	}

	return pBackGround;
}



void CBackGround::Free()
{
	Engine::CGameObject::Free();
}
