#include "pch.h"
#include "PoopHat.h"
#include "Player.h"

CPoopHat::CPoopHat(LPDIRECT3DDEVICE9 pGraphicDev)
    :CItem(pGraphicDev)
{
}

CPoopHat::~CPoopHat()
{
}

HRESULT CPoopHat::Ready_GameObject()
{
	CItem::Ready_GameObject();

	m_tInfo = { EQUIP,
		POOP,
		L"똥 모자",
		L"어디서 냄새가 나는 것이지? 나는 잘 모르겠어.",
		0, 1 };

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

void CPoopHat::LateReady_GameObject()
{
	CItem::LateReady_GameObject();

}

_int CPoopHat::Update_GameObject(const _float& fTimeDelta)
{
	return CItem::Update_GameObject(fTimeDelta);
}

void CPoopHat::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CItem::LateUpdate_GameObject(fTimeDelta);
}

void CPoopHat::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

void CPoopHat::Use_Item()
{
	m_pEquipHat = m_pPlayer->GetEquipHat();
	m_pEquipHat->SetHatTexture(m_pTextureCom);
}

HRESULT CPoopHat::Add_Component()
{
	CComponent* pComponent = NULL;

	// RC TEX
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PoopHat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TexturePoopHat", pComponent });

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

CPoopHat* CPoopHat::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPoopHat* pItem = new CPoopHat(pGraphicDev);

	if (FAILED(pItem->Ready_GameObject()))
	{
		Safe_Release(pItem);
		MSG_BOX("PoopHat Create Failed");
		return nullptr;
	}

	return pItem;
}
