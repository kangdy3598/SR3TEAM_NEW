#include "pch.h"
#include "ExploreHat.h"
#include "Player.h"

CExploreHat::CExploreHat(LPDIRECT3DDEVICE9 pGraphicDev)
	:CItem(pGraphicDev)
	, m_pEquipHat(nullptr)
{
}

CExploreHat::~CExploreHat()
{
}

HRESULT CExploreHat::Ready_GameObject()
{
	CItem::Ready_GameObject();

	m_tInfo = { EQUIP,
		EXPLORE,
		L"Å½Çè ¸ðÀÚ",
		L"Å½ÇèÇÒ ¶§ ÇÊ¼ö·Î Ã¬±â´Â ¸ðÀÚ. \
		Å½Çè°¡°¡ µÈ ±âºÐ..!!! \
		³¢¾æ -? È£!",
		0, 1 };

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

void CExploreHat::LateReady_GameObject()
{
	CItem::LateReady_GameObject();
}

_int CExploreHat::Update_GameObject(const _float& fTimeDelta)
{
	return CItem::Update_GameObject(fTimeDelta);
}

void CExploreHat::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CItem::LateUpdate_GameObject(fTimeDelta);
}

void CExploreHat::Render_GameObject()
{
	const Engine::_matrix* matTemp = m_pTransformCom->Get_WorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, matTemp);
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

void CExploreHat::Use_Item()
{
	m_pPlayer->SetItemMoveSpeed(50);
	m_pEquipHat = m_pPlayer->GetEquipHat();
	m_pEquipHat->SetHatTexture(m_pTextureCom);
}

HRESULT CExploreHat::Add_Component()
{
	CComponent* pComponent = NULL;

	// RC TEX
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	//Explore Hat
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ExploreHat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TextureExploreHat", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
	m_pTransformCom->m_vScale = { 55.f, 55.f, 1.f };
	m_pTransformCom->m_vInfo[INFO_POS] = { 0.f, 0.f, 0.1f };
	//0925Quick
	pComponent = m_pQuickTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformQuick", pComponent });


	return S_OK;
}

CExploreHat* CExploreHat::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CExploreHat* pItem = new CExploreHat(pGraphicDev);

	if (FAILED(pItem->Ready_GameObject()))
	{
		Safe_Release(pItem);
		MSG_BOX("ExploreHat Create Failed");
		return nullptr;
	}

	return pItem;

}
