#include "pch.h"
#include "TriAngle.h"
#include "Export_Utility.h"
#include "BackGround.h"

CTriAngle::CTriAngle(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
{
}

CTriAngle::~CTriAngle()
{
}

HRESULT CTriAngle::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pTransformCom->m_vScale = { 1.f,1.f,1.f };

    return S_OK;
  
}

_int CTriAngle::Update_GameObject(const _float& fTimeDelta)
{

	// 어떻게 할거냐면 네모의 위치벡터와 회전벡터를 같게할예정
	// 해당 방향벡터 구하면될거같음 

	//pScene 얻어오면될듯
	//CComponent* pComponent = nullptr; 
	//pComponent =  Engine::CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"Layer_Environment", L"BackGround", L"Com_Transform");

	m_pTransformCom->Get_Info(INFO_UP, &vUp);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_POS, &vPosition);

	//m_pTransformCom->Get_Info(INFO_POS, &vPosition);

	vUp = m_pPlayer->m_vUp - vUp;
	vRight = m_pPlayer->m_vRight- vRight;
	vLook = m_pPlayer->m_vLook -vLook;;
	vPosition = m_pPlayer->m_vPostion - vPosition;

	//vRight = m_pPlayer->m_vRight - vRight;
	//vLook = m_pPlayer->m_vLook - vLook;

	
	m_pTransformCom->Move_Pos(&vPosition, fTimeDelta, 1.f);
	//m_pTransformCom->Move_Pos(&vUp, fTimeDelta, 1.f);
	//m_pTransformCom->Move_Pos(&vRight, fTimeDelta, 1.f);
	//m_pTransformCom->Move_Pos(&vLook, fTimeDelta, 1.f);

	//m_pTransformCom->Move_Pos( &vUp, fTimeDelta, 10.f);
	//m_pTransformCom->Move_Pos( &vRight, fTimeDelta, 10.f);
	//m_pTransformCom->Move_Pos( &vLook, fTimeDelta, 10.f);


	//m_pTransformCom->Rotation(ROT_X, D3DXToRadian(180.f * fTimeDelta));
	//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
	//m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(180.f * fTimeDelta));


	//if (GetAsyncKeyState('W'))
	//{
	//	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
	//}
	//
	//if (GetAsyncKeyState('A'))
	//{
	//	m_pTransformCom->Rotation(ROT_X, D3DXToRadian(180.f * fTimeDelta));
	//}
	//
	//if (GetAsyncKeyState('D'))
	//{
	//	m_pTransformCom->Rotation(ROT_Z, D3DXToRadian(180.f * fTimeDelta));
	//}


	return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CTriAngle::LateUpdate_GameObject()
{
	Engine::CGameObject::LateUpdate_GameObject();
}

void CTriAngle::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CTriAngle::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CTriCol*>(Engine::Clone_Proto(L"Proto_TriCol"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

void CTriAngle::Key_Input(const _float& fTimeDelta)
{
}

CTriAngle* CTriAngle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTriAngle* pCTriAngle = new CTriAngle(pGraphicDev);

	if (FAILED(pCTriAngle->Ready_GameObject()))
	{
		Safe_Release(pCTriAngle);
		MSG_BOX("pBackGround Create Failed");
		return nullptr;
	}

	return  pCTriAngle;
}

void CTriAngle::Free()
{
}
