#include "pch.h"
#include "EnkyHat.h"
#include "Player.h"

CEnkyHat::CEnkyHat(LPDIRECT3DDEVICE9 pGraphicDev)
    :CItem(pGraphicDev)
{
}

CEnkyHat::~CEnkyHat()
{
}

HRESULT CEnkyHat::Ready_GameObject()
{
	CItem::Ready_GameObject();

	m_tInfo = { EQUIP,
		ENKY,
		L"엔키의 모자",
		L"바다의 신수 엔키의 형상을 띄고 있는 모자. 악신에 저항하고 세계를 보호하는 힘이 있다고 한다.",
		0, 1 };

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

void CEnkyHat::LateReady_GameObject()
{
	CItem::LateReady_GameObject();
}

_int CEnkyHat::Update_GameObject(const _float& fTimeDelta)
{
	return CItem::Update_GameObject(fTimeDelta);
}

void CEnkyHat::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CItem::LateUpdate_GameObject(fTimeDelta);
}

void CEnkyHat::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

void CEnkyHat::Use_Item()
{
	
	m_pEquipHat = m_pPlayer->GetEquipHat();
	m_pEquipHat->SetHatTexture(m_pTextureCom);
}

HRESULT CEnkyHat::Add_Component()
{
	CComponent* pComponent = NULL;

	// RC TEX
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	//Explore Hat
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_EnkyHat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TextureEnkyHat", pComponent });

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

CEnkyHat* CEnkyHat::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEnkyHat* pItem = new CEnkyHat(pGraphicDev);

	if (FAILED(pItem->Ready_GameObject()))
	{
		Safe_Release(pItem);
		MSG_BOX("EnkyHat Create Failed");
		return nullptr;
	}

	return pItem;
}
