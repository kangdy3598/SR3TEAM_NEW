#include "pch.h"
#include "Item.h"
#include "Player.h" //0924
#include "InvenUI.h"
#include "ItemUI.h"
#include "StoreUI.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , m_pPickUpButton(nullptr), m_pItemUI(nullptr)
{
    m_tInfo.bOnField = false;
}

CItem::~CItem()
{
}

HRESULT CItem::Ready_GameObject()
{
    m_bDontDestroy = true;

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    return S_OK;
}

void CItem::LateReady_GameObject()
{
    m_pPickUpButton = dynamic_cast<CPickUpButton*>(CPickUpButton::Create(m_pGraphicDev));
    NULL_CHECK_RETURN(m_pPickUpButton);

    m_pItemUI = dynamic_cast<CItemUI*>(CItemUI::Create(m_pGraphicDev));
    NULL_CHECK_RETURN(m_pItemUI);

    m_pStoreUI = dynamic_cast<CStoreUI*>(Engine::Get_GameObject(L"Layer_UI", L"Store_UI"));
    NULL_CHECK_RETURN(m_pStoreUI);

    m_pInven = dynamic_cast<CInvenUI*>(Engine::Get_GameObject(L"Layer_UI", L"Inven_UI"));
    NULL_CHECK_RETURN(m_pInven);

    m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
    NULL_CHECK_RETURN(m_pPlayer);
}

_int CItem::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_pPickUpButton)
    {
        m_pPickUpButton = dynamic_cast<CPickUpButton*>(CPickUpButton::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(m_pPickUpButton, 0);
    }
    if (!m_pItemUI)
    {
        m_pItemUI = dynamic_cast<CItemUI*>(CItemUI::Create(m_pGraphicDev));
        NULL_CHECK_RETURN(m_pItemUI, 0);
    }
    if (!m_pPlayer)
    {
        m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
        NULL_CHECK_RETURN(m_pPlayer, 0);
    }
    if (!m_pInven)
    {
        m_pInven = dynamic_cast<CInvenUI*>(Engine::Get_GameObject(L"Layer_UI", L"Inven_UI"));
        NULL_CHECK_RETURN(m_pInven, 0);
    }
    if (!m_pStoreUI)
    {
        m_pStoreUI = dynamic_cast<CStoreUI*>(Engine::Get_GameObject(L"Layer_UI", L"Store_UI"));
        NULL_CHECK_RETURN(m_pStoreUI, 0);
    }

    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    if (m_pPickUpButton)
    {
        // ¶³±¼ÀÏ¾ø´Â¾ÖµéÀº °Á null
        m_pPickUpButton->Update_GameObject(fTimeDelta);
    }
    if (m_pPickUpButton)
    {
        m_pItemUI->Update_GameObject(fTimeDelta); // ½ºÅä¾î¿¡¼­ ÃÖÃÊ È¹µæ ¾µÀÏ¾øÀÜ..
    }
    if (!m_tInfo.bOnField)
    { 
        Engine::Add_RenderGroup(RENDER_UI, this);
    }
    else if (m_tInfo.bOnField)
    {
        Engine::Add_RenderGroup(RENDER_ALPHA, this);
    }

    return iExit;
}

void CItem::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CItem::Render_GameObject()
{
}

void CItem::Render_QuickItem()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pQuickTransformCom->Get_WorldMatrix());
    m_pTextureCom->Set_Texture();
    m_pBufferCom->Render_Buffer();

    if (m_tInfo.eType == CONSUM)
    {
        _vec2 vCountPos;

        vCountPos.x = m_pQuickTransformCom->m_vInfo[INFO_POS].x + (WINCX * 0.5f) + 10;
        vCountPos.y = -(m_pQuickTransformCom->m_vInfo[INFO_POS].y) + (WINCY * 0.5f) + 10;

        wchar_t Division[32] = L"x";
        wchar_t ItemCount[32];

        swprintf(ItemCount, 32, L"%d", m_tInfo.iItemCount);

        wcscat_s(Division, 32, ItemCount);   // "x + °³¼ö"
        //Engine::Render_Font(L"Font_Ogu10", Division, &vCountPos, D3DXCOLOR(0.f, 0.1f, 0.1f, 1.f));
        Engine::Render_Font(L"Font_OguBold14", Division, &vCountPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
    }
}

void CItem::Render_ItemView()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pViewTransformCom->Get_WorldMatrix());
    m_pTextureCom->Set_Texture();
    m_pBufferCom->Render_Buffer();
}

void CItem::Set_DropItem(_vec3 _ItemPos)
{
    m_tInfo.bOnField = true;
    m_pTransformCom->m_vScale = { 15.f, 15.f, 20.f };
    Set_ItemPos(_ItemPos);
}

HRESULT CItem::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pViewTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformItemView", pComponent });
    m_pViewTransformCom->m_vScale = { 100.f, 100.f, 1.f };
    m_pViewTransformCom->m_vInfo[INFO_POS] = { 430.f, 100.f, 0.1f };

    pComponent = m_pPriceTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PriceUI"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_TexturePriceUI", pComponent });

    pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_pColliderCom->SetGameObjectPtr(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

    return S_OK;
}

CItem* CItem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CItem* pItem = new CItem(pGraphicDev);

    if (FAILED(pItem->Ready_GameObject()))
    {
        Safe_Release(pItem);
        MSG_BOX("Item Create Failed");
        return nullptr;
    }

    return pItem;
}

void CItem::Free()
{
    Engine::CGameObject::Free();
}
