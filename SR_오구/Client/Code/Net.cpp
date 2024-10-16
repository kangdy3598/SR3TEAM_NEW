#include "pch.h"
#include "Net.h"
#include "Player.h"
#include "ItemUI.h"

_bool CNet::g_Acquired(false);

CNet::CNet(LPDIRECT3DDEVICE9 pGraphicDev)
    :CItem(pGraphicDev)
{
}

CNet::~CNet()
{
}

HRESULT CNet::Ready_GameObject()
{
	CItem::Ready_GameObject();

	m_tInfo = { QUEST,
		NET,
		L"잠자리채",
		L"잠자리를 잡을 수 있는 채. 이외의 용도로 사용할 수 있을지도??ㅋ_ㅋ??",
		1, 1 };

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

void CNet::LateReady_GameObject()
{
	CItem::LateReady_GameObject();
}

_int CNet::Update_GameObject(const _float& fTimeDelta)
{
	return CItem::Update_GameObject(fTimeDelta);
}

void CNet::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CItem::LateUpdate_GameObject(fTimeDelta);
}

void CNet::Render_GameObject()
{
	if (m_tInfo.bOnField)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pTextureCom->Set_Texture();
		m_pBufferCom->Render_Buffer();
		//m_pColliderCom->Render_Buffer();

		return;
	}
	else if (m_pInven->Get_CurFilter() == m_tInfo.eType
		&& m_pPlayer->GetPlayerInven()
		&& !m_tInfo.bOnField)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pTextureCom->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}
}

void CNet::OnCollision(CGameObject* _pOther)
{
	if (CNet::g_Acquired == true)
	{
		m_pInven->Add_Item(dynamic_cast<CItem*>(this));
		//아이템 획득 이펙트 발생
		return;
	}

	m_pPickUpButton->CallButton(true);

	if (GetKeyDown(DIK_A)) //줍기
	{
		CNet::g_Acquired = true;
		m_pItemUI->CallItemUI(true);
		m_pItemUI->Set_Texture(m_pTextureCom);
		m_pItemUI->Set_Text(m_tInfo);
		m_pInven->Add_Item(dynamic_cast<CItem*>(this));
	}
}

HRESULT CNet::Add_Component()
{
	CComponent* pComponent = NULL;

	// RC TEX
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	//SmallFruit
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Net"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TextureNet", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
	m_pTransformCom->m_vScale = { 55.f, 55.f, 1.f };
	m_pTransformCom->m_vInfo[INFO_POS] = { 0.f, 0.f, 0.1f };

	return S_OK;
}

CNet* CNet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNet* pItem = new CNet(pGraphicDev);

	if (FAILED(pItem->Ready_GameObject()))
	{
		Safe_Release(pItem);
		MSG_BOX("Net Create Failed");
		return nullptr;
	}

	return pItem;
}
