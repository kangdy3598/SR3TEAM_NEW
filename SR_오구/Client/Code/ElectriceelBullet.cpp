#include "pch.h"
#include "ElectriceelBullet.h"
#include "Export_Utility.h"

CElectriceelBullet::CElectriceelBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:Engine::CGameObject(pGraphicDev)
	, name(L"")
	, m_bEffect(false)
	, m_iCount(0)
	, m_vDir(0.f,0.f,0.f)
{
	m_dwtime = GetTickCount64();
}

CElectriceelBullet::~CElectriceelBullet()
{
}

HRESULT CElectriceelBullet::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vScale = { 10.f,15.f,10.f };
	SetObjectType(OBJ_TYPE::HURT_ABLE2);	
	//m_pTransformCom->Set_Pos(650.f, 15.f, 650.f);
	return S_OK;
}

_int CElectriceelBullet::Update_GameObject(const _float& fTimeDelta)
{
	m_pTransformCom->Move_Pos(&m_vDir, fTimeDelta, 200.f);

	Add_RenderGroup(RENDER_ALPHA, this);
	return Engine::CGameObject::Update_GameObject(fTimeDelta);

}

void CElectriceelBullet::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CElectriceelBullet::Render_GameObject()
{
	if (m_dwtime + 4500 < GetTickCount64())
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
	}

	else
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		m_pTextureCom->Set_Texture(0);
		m_pBufferCom->Render_Buffer();

		//맵툴 작성시 끄기 
		////m_pBoundBox->Render_Buffer();   
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
	return;
}

HRESULT CElectriceelBullet::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });
	
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pBoundBox = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBoundBox->SetGameObjectPtr(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Electric_Bullet"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CElectriceelBullet* CElectriceelBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CElectriceelBullet* pElectriceelBullet = new CElectriceelBullet(pGraphicDev);

	if (FAILED(pElectriceelBullet->Ready_GameObject()))
	{
		Safe_Release(pElectriceelBullet);
		MSG_BOX("pObject Create Failed");
		return nullptr;
	}

	return pElectriceelBullet;
}

void CElectriceelBullet::Free()
{
	Engine::CGameObject::Free();	
}

void CElectriceelBullet::OnCollision(CGameObject* _pOther)
{
	int a = 4;
}
