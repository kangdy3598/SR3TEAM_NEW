#include "pch.h"
#include "ItemSelector.h"

#include "Export_Utility.h"
#include "Export_System.h"

#include "InvenUI.h"

CItemSelector::CItemSelector(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
	, m_iCurIdx(-5)
{
}

CItemSelector::~CItemSelector()
{
}

HRESULT CItemSelector::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    return S_OK;
}

_int CItemSelector::Update_GameObject(const _float& fTimeDelta)
{

	//레이트 레디 필요
	m_pInven = dynamic_cast<CInvenUI*>(Engine::Get_GameObject(L"Layer_UI", L"Inven_UI"));
	NULL_CHECK_RETURN(m_pInven, 0);


	_int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

	Key_Input(fTimeDelta);

	Update_ItemInfo();

	Engine::Add_RenderGroup(RENDER_UI, this);

	return iExit;
}

void CItemSelector::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CItemSelector::Render_GameObject()
{
	if (m_iCurIdx < 0)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	_vec2 ItemNamePos(930.f, 442.f);
	Engine::Render_Font(L"Font_Ogu24", m_tCopyInfo.pName, &ItemNamePos, D3DXCOLOR(0.999f, 0.98f, 0.9f, 0.7f));

	_vec2 ItemInfoPos(930.f, 492.f);
	_vec2 vXYSize(290.f, 400.f);
	Engine::Render_Font(L"Font_Ogu22", m_tCopyInfo.pInfo, &ItemInfoPos, D3DXCOLOR(0.999f, 0.98f, 0.9f, 0.7f), vXYSize);

	// 버튼
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransButtonCom->Get_WorldMatrix());
	m_pTexButtonCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	_vec2 QuipButton(995.f, 688.5f);
	Engine::Render_Font(L"Font_Ogu14", L"퀵슬롯 설정", &QuipButton, D3DXCOLOR(0.999f, 0.98f, 0.9f, 1.f));
	_vec2 QuickButton(1140.f, 688.5f);
	Engine::Render_Font(L"Font_Ogu14", L"장착하기", &QuickButton, D3DXCOLOR(0.999f, 0.98f, 0.9f, 1.f));

}

HRESULT CItemSelector::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PipeCursorTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureCursor", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformCursor", pComponent });
	m_pTransformCom->m_vScale = { 70.f, 70.f, 1.f };

	pComponent = m_pTexButtonCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_EquipOnOffButton"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureEquipOnOffButton", pComponent });

	pComponent = m_pTransButtonCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformEquipOnOffButton", pComponent });
	m_pTransButtonCom->m_vScale = { 132.5f, 21.5f, 1.f };
	m_pTransButtonCom->m_vInfo[INFO_POS] = { 475.f, -338.f, 0.1f };

	return S_OK;
}

void CItemSelector::Key_Input(const _float& fTimeDelta)
{
	m_iPrevIdx = m_iCurIdx;

	if (Engine::GetKeyDown(DIK_DOWN))
	{
		if (m_iCurIdx >= 10)
			return;
		m_iCurIdx += 5;
	}
	else if (m_iCurIdx < 0)
		return;

	if (Engine::GetKeyDown(DIK_UP))
	{	
		m_iCurIdx -= 5;
	}
	if (Engine::GetKeyDown(DIK_LEFT))
	{
		if (m_iCurIdx % 5 == 0)
			return;
		--m_iCurIdx;
	}
	if (Engine::GetKeyDown(DIK_RIGHT))
	{
		if (m_iCurIdx % 5 == 4)
			return;
		++m_iCurIdx;
	}

	if (Engine::GetKeyDown(DIK_I))
	{
		m_iCurIdx = -5;
	}
}

void CItemSelector::Update_ItemInfo()
{
	if(m_iCurIdx < 0)
		return;

	CItem::ITEMTYPE eFilter = m_pInven->Get_CurFilter();
	//현재 어떤 필터로 인벤토리가 열려 있는지 

	if (m_pInven->Is_Empty(eFilter))
		return;

	_int	iItemCount = m_pInven->Get_FilterItemCount(eFilter);

	if (iItemCount <= m_iCurIdx)
	{
		m_iCurIdx = m_iPrevIdx;
		return;
	}

	_vec3 vPos;
	vPos = m_pInven->Get_CurItemPos(eFilter, m_iCurIdx);
	m_pTransformCom->m_vInfo[INFO_POS] = vPos;

	m_tCopyInfo = m_pInven->Get_ItemInfo(eFilter, m_iCurIdx);
}

CItemSelector* CItemSelector::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CItemSelector* pItemSelector = new CItemSelector(pGraphicDev);

	if (FAILED(pItemSelector->Ready_GameObject()))
	{
		Safe_Release(pItemSelector);
		MSG_BOX("pItemSelector Create Failed");
		return nullptr;
	}

	return pItemSelector;
}

void CItemSelector::Free()
{
	Engine::CGameObject::Free();
}
