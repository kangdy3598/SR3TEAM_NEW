#include "pch.h"
#include "MiddleHerb.h"
#include "Player.h"
#include "ItemUI.h"

_bool CMiddleHerb::g_Acquired(false);

CMiddleHerb::CMiddleHerb(LPDIRECT3DDEVICE9 pGraphicDev)
    :CItem(pGraphicDev)
{
}

CMiddleHerb::~CMiddleHerb()
{
}

HRESULT CMiddleHerb::Ready_GameObject()
{
	CItem::Ready_GameObject();

	m_tInfo = { CONSUM,
		MIDDLE_HERB,
		L"질풍의 약초",
		L"2등급 약초. 이동속도가 10초동안 상승한다.",
		300, 1 };

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

void CMiddleHerb::LateReady_GameObject()
{
	CItem::LateReady_GameObject();
}

_int CMiddleHerb::Update_GameObject(const _float& fTimeDelta)
{
	return CItem::Update_GameObject(fTimeDelta);
}

void CMiddleHerb::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CItem::LateUpdate_GameObject(fTimeDelta);
}

void CMiddleHerb::Render_GameObject()
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
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		m_pTextureCom->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pCountRCTransformCom->m_vInfo[INFO_POS].x = m_pTransformCom->m_vInfo[INFO_POS].x + 36;
		m_pCountRCTransformCom->m_vInfo[INFO_POS].y = m_pTransformCom->m_vInfo[INFO_POS].y - 44;
		m_pCountRCTransformCom->m_vScale = { 40.f, 17.f, 1.f };

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pCountRCTransformCom->Get_WorldMatrix());
		m_pPriceTextureCom->Set_Texture();
		m_pBufferCom->Render_Buffer();

		_vec2 vCountPos;

		vCountPos.x = m_pTransformCom->m_vInfo[INFO_POS].x + (WINCX * 0.5f) + 28;
		vCountPos.y = -(m_pTransformCom->m_vInfo[INFO_POS].y) + (WINCY * 0.5f) + 34;

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


void CMiddleHerb::Use_Item()
{
	m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
	NULL_CHECK_RETURN(m_pPlayer);
	
	Engine::Play_Sound(L"SFX_785_OguAttackUp.wav", SOUND_EFFECT, 1.f);

	m_pPlayer->SetSpeedTime(10);
	m_tInfo.iItemCount--;
}

void CMiddleHerb::OnCollision(CGameObject* _pOther)
{
	if (CMiddleHerb::g_Acquired == true)
	{
		m_pInven->Add_Item(dynamic_cast<CItem*>(this));
		//아이템 획득 이펙트 발생
		return;
	}

	m_pPickUpButton->CallButton(true);

	if (GetKeyDown(DIK_A)) //줍기
	{
		CMiddleHerb::g_Acquired = true;
		m_pItemUI->CallItemUI(true);
		m_pItemUI->Set_Texture(m_pTextureCom);
		m_pItemUI->Set_Text(m_tInfo);
		m_pInven->Add_Item(dynamic_cast<CItem*>(this));
	}
}

HRESULT CMiddleHerb::Add_Component()
{
	CComponent* pComponent = NULL;

	// RC TEX
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	//SmallFruit
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MiddleHerb"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_TextureMiddleHerb", pComponent });

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

CMiddleHerb* CMiddleHerb::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMiddleHerb* pItem = new CMiddleHerb(pGraphicDev);

	if (FAILED(pItem->Ready_GameObject()))
	{
		Safe_Release(pItem);
		MSG_BOX("MiddleHerb Create Failed");
		return nullptr;
	}

	return pItem;
}
