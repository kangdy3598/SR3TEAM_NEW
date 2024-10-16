#include "pch.h"
#include "TestObject.h"
#include "Export_Utility.h"

CTestObject::CTestObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)	
	,m_pTex(nullptr)
{
}

CTestObject::~CTestObject()
{
}

HRESULT CTestObject::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/Npc/MonkeyIDLE.png", &m_pTex);	

	
	m_pAnimatorCom->CreateAnimation(L"Test", m_pTex, _vec2(0.f, 0.f), _vec2(136.5f, 145.f), _vec2(136.5f, 0.f), 0.5f, 14);

	m_pAnimatorCom->Play(L"Test", false);

	m_pTransformCom->m_vScale = { 50.f,50.f,50.f };
	m_pTransformCom->Set_Pos(200.f, 35.f, 700.f);

	return S_OK;
}

_int CTestObject::Update_GameObject(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);	

	

	return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CTestObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
	

	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTestObject::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTexture(0, m_pTex);


	m_pAnimatorCom->render();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	
}

HRESULT CTestObject::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));	
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animation_Buffer", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));	
	NULL_CHECK_RETURN(pComponent, E_FAIL);	
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });			

	return S_OK;	
}

void CTestObject::Key_Input(const _float& fTimeDelta)
{
}

CTestObject* CTestObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTestObject* pTestObject = new CTestObject(pGraphicDev);

	if (FAILED(pTestObject->Ready_GameObject()))
	{
		Safe_Release(pTestObject);
		MSG_BOX("pTestObject Create Failed");
		return nullptr;
	}

	return pTestObject;
}

void CTestObject::Free()
{
	Engine::CGameObject::Free();	
}
