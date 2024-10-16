#include "pch.h"
#include "AppleHat.h"
#include "Player.h"

CAppleHat::CAppleHat(LPDIRECT3DDEVICE9 pGraphicDev)
    :CItem(pGraphicDev)
{
}

CAppleHat::~CAppleHat()
{
}

HRESULT CAppleHat::Ready_GameObject()
{
	CItem::Ready_GameObject();

	m_tInfo = { EQUIP,
		APPLE,
		L"사과 모자",
		L"맛있는 사과 모양의 모자이다.",
		0, 1 };

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

void CAppleHat::LateReady_GameObject()
{
	CItem::LateReady_GameObject();
}

_int CAppleHat::Update_GameObject(const _float& fTimeDelta)
{
	return CItem::Update_GameObject(fTimeDelta);
}

void CAppleHat::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CItem::LateUpdate_GameObject(fTimeDelta);
}

void CAppleHat::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
}

void CAppleHat::Use_Item()
{
	m_pEquipHat = m_pPlayer->GetEquipHat();
	m_pEquipHat->SetHatTexture(m_pTextureCom);
}

HRESULT CAppleHat::Add_Component()
{
	CComponent* pComponent = NULL;

	// RC TEX
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_AppleHat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TextureApple", pComponent });

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

CAppleHat* CAppleHat::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CAppleHat* pItem = new CAppleHat(pGraphicDev);

	if (FAILED(pItem->Ready_GameObject()))
	{
		Safe_Release(pItem);
		MSG_BOX("AppleHat Create Failed");
		return nullptr;
	}

	return pItem;
}
