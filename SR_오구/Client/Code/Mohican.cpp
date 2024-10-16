#include "pch.h"
#include "Mohican.h"

CMohican::CMohican(LPDIRECT3DDEVICE9 pGraphicDev)
	:CItem(pGraphicDev)
{
}

CMohican::~CMohican()
{
}

HRESULT CMohican::Ready_GameObject()
{
	CItem::Ready_GameObject();

	m_tInfo = { EQUIP,
		MOHICAN,
		 L"모히칸 헤어",
		 L"강해진 것만 같은 기분이 든다....",
		100, 1 };

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

void CMohican::LateReady_GameObject()
{
	CItem::LateReady_GameObject();
}

_int CMohican::Update_GameObject(const _float& fTimeDelta)
{
	return CItem::Update_GameObject(fTimeDelta);
}

void CMohican::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CItem::LateUpdate_GameObject(fTimeDelta);
}

void CMohican::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CMohican::Add_Component()
{
	CComponent* pComponent = NULL;

	// RC TEX
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	//Mohican
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Mohican"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TextureMohican", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
	m_pTransformCom->m_vScale = { 55.f, 55.f, 1.f };
	m_pTransformCom->m_vInfo[INFO_POS] = { 0.f, 0.f, 0.f };
	//0925Quick
	pComponent = m_pQuickTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformQuick", pComponent });

	return S_OK;
}

CMohican* CMohican::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMohican* pItem = new CMohican(pGraphicDev);

	if (FAILED(pItem->Ready_GameObject()))
	{
		Safe_Release(pItem);
		MSG_BOX("Mohican Create Failed");
		return nullptr;
	}

	return pItem;
}
