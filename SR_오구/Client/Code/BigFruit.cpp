#include "pch.h"
#include "BigFruit.h"
#include "Player.h"
#include "ItemUI.h"

_bool CBigFruit::g_Acquired(false);

CBigFruit::CBigFruit(LPDIRECT3DDEVICE9 pGraphicDev)
    :CItem(pGraphicDev)
{
}

CBigFruit::~CBigFruit()
{
}

HRESULT CBigFruit::Ready_GameObject()
{
	CItem::Ready_GameObject();

	m_tInfo = { CONSUM,
		BIG_FRUIT,
		L"튼튼 열매",
		L"1등급 열매. 체력이 3 회복된다.",
		300, 1 };

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

void CBigFruit::LateReady_GameObject()
{
	CItem::LateReady_GameObject();
}

_int CBigFruit::Update_GameObject(const _float& fTimeDelta)
{
	return CItem::Update_GameObject(fTimeDelta);
}

void CBigFruit::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CItem::LateUpdate_GameObject(fTimeDelta);
}

void CBigFruit::Render_GameObject()
{
	if (m_tInfo.bOnField)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pTextureCom->Set_Texture();
		m_pBufferCom->Render_Buffer();
		//m_pColliderCom->Render_Buffer();

		return;
	}
	else if (m_pPlayer->GetVisitingStore())
	{
		//아이템
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pStoreTransformCom->Get_WorldMatrix());
		m_pTextureCom->Set_Texture();
		m_pBufferCom->Render_Buffer();

		//가격태그
		m_pCountRCTransformCom->m_vInfo[INFO_POS].x = m_pStoreTransformCom->m_vInfo[INFO_POS].x + 36;
		m_pCountRCTransformCom->m_vInfo[INFO_POS].y = m_pStoreTransformCom->m_vInfo[INFO_POS].y - 44;
		m_pCountRCTransformCom->m_vScale = { 40.f, 17.f, 1.f };

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pCountRCTransformCom->Get_WorldMatrix());
		m_pPriceTextureCom->Set_Texture();
		m_pBufferCom->Render_Buffer();

		_vec2 vCountPos;

		vCountPos.x = m_pStoreTransformCom->m_vInfo[INFO_POS].x + (WINCX * 0.5f) + 28;
		vCountPos.y = -(m_pStoreTransformCom->m_vInfo[INFO_POS].y) + (WINCY * 0.5f) + 34;

		wchar_t ItemCount[32];

		swprintf(ItemCount, 32, L"%d", m_tInfo.iPrice);

		Engine::Render_Font(L"Font_OguBold24", ItemCount, &vCountPos, D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));

	}
	else if (m_pInven->Get_CurFilter() == m_tInfo.eType
		&& m_pPlayer->GetPlayerInven()
		&& !m_tInfo.bOnField)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pTextureCom->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pCountRCTransformCom->m_vInfo[INFO_POS].x = m_pTransformCom->m_vInfo[INFO_POS].x + 46;
		m_pCountRCTransformCom->m_vInfo[INFO_POS].y = m_pTransformCom->m_vInfo[INFO_POS].y - 44;
		m_pCountRCTransformCom->m_vScale = { 20.f, 17.f, 1.f };

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pCountRCTransformCom->Get_WorldMatrix());
		m_pCountRCTextureCom->Set_Texture();
		m_pBufferCom->Render_Buffer();

		_vec2 vCountPos;

		vCountPos.x = m_pTransformCom->m_vInfo[INFO_POS].x + (WINCX * 0.5f) + 34;
		vCountPos.y = -(m_pTransformCom->m_vInfo[INFO_POS].y) + (WINCY * 0.5f) + 34;

		wchar_t Division[32] = L"x";
		wchar_t ItemCount[32];

		swprintf(ItemCount, 32, L"%d", m_tInfo.iItemCount);

		wcscat_s(Division, 32, ItemCount);   // "x + 개수"
		Engine::Render_Font(L"Font_OguBold24", Division, &vCountPos, D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f));
	}
}

void CBigFruit::OnCollision(CGameObject* _pOther)
{
	if (CBigFruit::g_Acquired == true)
	{
		m_pInven->Add_Item(dynamic_cast<CItem*>(this));
		//아이템 획득 이펙트 발생
		return;
	}

	m_pPickUpButton->CallButton(true);

	if (GetKeyDown(DIK_A)) //줍기
	{
		CBigFruit::g_Acquired = true;
		m_pItemUI->CallItemUI(true);
		m_pItemUI->Set_Texture(m_pTextureCom);
		m_pItemUI->Set_Text(m_tInfo);
		m_pInven->Add_Item(dynamic_cast<CItem*>(this));
	}
}

void CBigFruit::Use_Item()
{
	m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	NULL_CHECK_RETURN(m_pPlayer);

	if (m_pPlayer->GetPlayerHP().iMaxHP == m_pPlayer->GetPlayerHP().iCurHP)
		return;

	Engine::Play_Sound(L"SFX_373_OguEat.wav", SOUND_EFFECT, 0.5);

	m_pPlayer->SetPlayerCurHP(3);
	m_tInfo.iItemCount--;
}

HRESULT CBigFruit::Add_Component()
{
	CComponent* pComponent = NULL;

	// RC TEX
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	//SmallFruit
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BigFruit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TextureBigFruit", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
	m_pTransformCom->m_vScale = { 55.f, 55.f, 1.f };
	m_pTransformCom->m_vInfo[INFO_POS] = { 0.f, 0.f, 0.1f };
	//0925Quick
	pComponent = m_pQuickTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformQuick", pComponent });


	//CountRC
	pComponent = m_pCountRCTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_CountRC"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TextureCountRC", pComponent });

	pComponent = m_pCountRCTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformCountRC", pComponent });
	m_pCountRCTransformCom->m_vScale = { 20.f, 17.f, 1.f };
	m_pCountRCTransformCom->m_vInfo[INFO_POS] = { 0.f, 0.f, 0.1f };

	return S_OK;
}

CBigFruit* CBigFruit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBigFruit* pItem = new CBigFruit(pGraphicDev);

	if (FAILED(pItem->Ready_GameObject()))
	{
		Safe_Release(pItem);
		MSG_BOX("BigFruit Create Failed");
		return nullptr;
	}

	return pItem;
}
