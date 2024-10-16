#include "pch.h"
#include "Penguin.h"

CPenguin::CPenguin(LPDIRECT3DDEVICE9 pGraphicDev)
    :CItem(pGraphicDev)
{
}

CPenguin::~CPenguin()
{
}

HRESULT CPenguin::Ready_GameObject()
{
	CItem::Ready_GameObject();

	m_tInfo = { EQUIP,
		PENGUIN,
		L"Æë±Ï ¸ðÀÚ",
		L"Æë±Ï Èä³»¸¦ ³¾ ¼ö ÀÖ´Ù.",
		0, 1 };

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

void CPenguin::LateReady_GameObject()
{
	CItem::LateReady_GameObject();
}

_int CPenguin::Update_GameObject(const _float& fTimeDelta)
{
	return CItem::Update_GameObject(fTimeDelta);

}

void CPenguin::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CItem::LateUpdate_GameObject(fTimeDelta);
}

void CPenguin::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CPenguin::Add_Component()
{
	CComponent* pComponent = NULL;

	// RC TEX
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	//Explore Hat
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Penguin"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TexturePenguin", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
	m_pTransformCom->m_vScale = { 55.f, 55.f, 1.f };
	m_pTransformCom->m_vInfo[INFO_POS] = { 0.f, 0.f, 0.1f };

	pComponent = m_pQuickTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformPenguin", pComponent });

	return S_OK;
}

CPenguin* CPenguin::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPenguin* pItem = new CPenguin(pGraphicDev);

	if (FAILED(pItem->Ready_GameObject()))
	{
		Safe_Release(pItem);
		MSG_BOX("Penguin Create Failed");
		return nullptr;
	}

	return pItem;
}
