#include "pch.h"
#include "WaterFall.h"
#include "Export_Utility.h"
#include "ResMgr.h"

CWaterFall::CWaterFall(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
	,m_vDir(0.f,0.f,0.f)
	,name(L"")
	,m_bEffect(false)
	,m_iCount(0)
{

}

CWaterFall::~CWaterFall()
{
}

HRESULT CWaterFall::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	IDirect3DTexture9* pEffectTexture1 = CResMgr::GetInstance()->GetEffectTexture()[1];	

	m_pAnimatorCom->CreateAnimation(L"Water_Fall", pEffectTexture1, _vec2(0.f, 0.f), _vec2(1024.f, 1024.f), _vec2(1024.f, 0.f), 0.2f, 3);
	
	//m_pAnimatorCom->Play(L"Water_Fall",true);

	//m_pTransformCom->Set_Pos(500.f, 100.f, 500.f);
	//m_pTransformCom->m_vScale = { 50.f, 100.f,10.f };
	SetObjectType(OBJ_TYPE::HURT_ABLE2);	

	m_pTransformCom->m_vScale = { 200.f, 1000.f, 20.f };

	m_dwtime = GetTickCount64();	

	return S_OK;	
}

void CWaterFall::LateReady_GameObject()
{
	
	m_pTransformCom->Rotation(ROT_X, 90 * 3.14f / 180.f);

	if (m_vDir == _vec3{ -1.f, 0.f, -1.f })
	{
		m_pTransformCom->Rotation(ROT_Y, 45 * 3.14f / 180.f);
	}

	else if (m_vDir == _vec3{ 1.f, 0.f, -1.f })
	{
		m_pTransformCom->Rotation(ROT_Y, -45 * 3.14f / 180.f);
	}

	else if (m_vDir == _vec3{ 0.f, 0.f, -1.f })
	{
		int a = 4;
	}

	else if (m_vDir == _vec3{ 1.f, 0.f, 1.f })
	{
		m_pTransformCom->Rotation(ROT_Y, -135 * 3.14f / 180.f);
	}

	else if (m_vDir == _vec3{ 0.f, 0.f, 1.f })
	{
		m_pTransformCom->Rotation(ROT_Y, -180 * 3.14f / 180.f);
	}

	else if (m_vDir == _vec3{ -1.f, 0.f, 1.f })
	{
		m_pTransformCom->Rotation(ROT_Y, 135 * 3.14f / 180.f);
	}

		
	
}

_int CWaterFall::Update_GameObject(const _float& fTimeDelta)
{
	//수정
	
	
	if (m_dwtime + 1000 < GetTickCount64())
	{
		m_iCount++;
		m_pAnimatorCom->Play(L"Water_Fall", true);
		m_pTransformCom->Move_Pos(&m_vDir, fTimeDelta, 150.f);
		m_pTransformCom->m_vScale = { 50.f, 100.f,10.f };

		if(m_iCount == 1)
		{
			m_pTransformCom->Rotation(ROT_X, -90 * 3.14f / 180.f);
		}
		

		Add_RenderGroup(RENDER_ALPHA, this);	
		m_bEffect = true;
	}

	else
	{
		Add_RenderGroup(RENDER_TRANSLUCNET, this);
	}


	return Engine::CGameObject::Update_GameObject(fTimeDelta);		
}

void CWaterFall::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);	
}

void CWaterFall::Render_GameObject()
{
	if (m_dwtime + 4000 < GetTickCount64())
	{
		Engine::CGameObject::Free();
		map<const _tchar*, CLayer*>& mapLayer = CManagement::GetInstance()->GetCurScenePtr()->GetLayerMapPtr();
		//그니깐 레이어에서 삭제해줘야한다는것.	
		CLayer* pLayer = nullptr;

		for (auto iter = mapLayer.begin(); iter != mapLayer.end(); ++iter)
		{
			const _tchar* layerKey = iter->first;

			if (_tcscmp(layerKey, _T("Layer_GameLogic")) == 0)
			{
				pLayer = iter->second;
			}
		}
		map<const _tchar*, CGameObject*>& pMap = pLayer->GetLayerGameObjectPtr();

		auto iter = find_if(pMap.begin(), pMap.end(), CTag_Finder(name.c_str()));

		if (iter != pMap.end())
		{
			pMap.erase(iter);
		}

		return;
	}



	if(m_bEffect ==false)
	{
		/*if (m_iCount == 0)
		{
			m_pTransformCom->Rotation(ROT_X, 90 * 3.14f / 180.f);
			
			if(m_vDir == _vec3{-1.f, 0.f, -1.f})
			{
				m_pTransformCom->Rotation(ROT_Y, 45 * 3.14f / 180.f);
			}

			else if (m_vDir == _vec3{ 1.f, 0.f, -1.f })
			{
				m_pTransformCom->Rotation(ROT_Y, -45 * 3.14f / 180.f);
			}

			else if (m_vDir == _vec3{ 0.f, 0.f, -1.f })
			{
				int a = 4;
			}

			else if (m_vDir == _vec3{ 1.f, 0.f, 1.f })
			{
				m_pTransformCom->Rotation(ROT_Y, -135 * 3.14f / 180.f);
			}

			else if (m_vDir == _vec3{ 0.f, 0.f, 1.f })
			{
				m_pTransformCom->Rotation(ROT_Y, -180 * 3.14f / 180.f);
			}

			else if (m_vDir == _vec3{ -1.f, 0.f, 1.f })
			{
				m_pTransformCom->Rotation(ROT_Y, 135 * 3.14f / 180.f);
			}

			m_iCount++;	
		}*/

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pTextureCom->Set_Texture(0);	
		m_pBufferCom->Render_Buffer();	

		//맵툴 작성시 끄기 
		////m_pBoundBox->Render_Buffer();   
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		return;
	}

	else
	{

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		

		m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetEffectTexture()[1]);
		m_pAnimatorCom->render();
		////m_pBoundBox->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
	
}



HRESULT CWaterFall::Add_Component()
{
	CComponent* pComponent = NULL;	

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Attack_Line"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);	
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);	
	m_pBoundBox->SetGameObjectPtr(this);		
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });	

	return S_OK;
}

HRESULT CWaterFall::SetUp_Material()
{
	return E_NOTIMPL;
}

void CWaterFall::Key_Input(const _float& fTimeDelta)
{
}

CWaterFall* CWaterFall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWaterFall* pWaterFall = new CWaterFall(pGraphicDev);

	if (FAILED(pWaterFall->Ready_GameObject()))
	{
		Safe_Release(pWaterFall);
		MSG_BOX("pObject Create Failed");
		return nullptr;
	}

	return pWaterFall;
}

void CWaterFall::Free()
{
	Engine::CGameObject::Free();
}
