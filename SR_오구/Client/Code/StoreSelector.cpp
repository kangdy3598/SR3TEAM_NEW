#include "pch.h"
#include "StoreSelector.h"
#include "StoreUI.h"

CStoreSelector::CStoreSelector(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , m_iCurIdx(-5)
{
}

CStoreSelector::~CStoreSelector()
{
}

HRESULT CStoreSelector::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/UI/ItemCursorUI.png", &m_pTex);
    m_pAnimatorCom->CreateAnimation(L"StoreSelector", m_pTex, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.12f, 1);

    return S_OK;
}

void CStoreSelector::LateReady_GameObject()
{
    m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
    NULL_CHECK_RETURN(m_pPlayer);

    m_pInven = dynamic_cast<CInvenUI*>(Engine::Get_GameObject(L"Layer_UI", L"Inven_UI"));
    NULL_CHECK_RETURN(m_pInven);

    m_pStore = dynamic_cast<CStoreUI*>(Engine::Get_GameObject(L"Layer_UI", L"Store_UI"));
    NULL_CHECK_RETURN(m_pStore);
}

_int CStoreSelector::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    Key_Input(fTimeDelta);

    Update_ItemInfo();

    Engine::Add_RenderGroup(RENDER_UI, this);

    return iExit;
}

void CStoreSelector::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CStoreSelector::Render_GameObject()
{
    if (m_iCurIdx < 0)
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetTexture(0, m_pTex);
    m_pAnimatorCom->Play(L"StoreSelector", true);
    m_pAnimatorCom->render();

    m_pItem->Render_ItemView(); // 추가

    _vec2 ItemNamePos(930.f, 442.f);
    Engine::Render_Font(L"Font_Ogu24", m_tCopyInfo.pName, &ItemNamePos, D3DXCOLOR(0.999f, 0.98f, 0.9f, 0.7f));

    _vec2 ItemInfoPos(930.f, 492.f);
    _vec2 vXYSize(290.f, 400.f);
    Engine::Render_Font(L"Font_Ogu22", m_tCopyInfo.pInfo, &ItemInfoPos, D3DXCOLOR(0.999f, 0.98f, 0.9f, 0.7f), TEXT_DEFAULT, vXYSize);

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransButtonCom->Get_WorldMatrix());
    m_pTexButtonCom->Set_Texture();
    m_pBufferCom->Render_Buffer();

}

HRESULT CStoreSelector::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTexButtonCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BuyButton"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureBuyButton", pComponent });

    pComponent = m_pTransButtonCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformBuyButton", pComponent });
    m_pTransButtonCom->m_vScale = { 63.f, 27.f, 1.f };
    m_pTransButtonCom->m_vInfo[INFO_POS] = { 532.f, -270.f, 0.1f };

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 75.f, 70.f, 1.f };

    return S_OK;
}

void CStoreSelector::Key_Input(const _float& fTimeDelta)
{
    m_iPrevIdx = m_iCurIdx;

    if (Engine::GetKeyDown(DIK_DOWN))
    {
        Engine::Play_Sound(L"SFX_70_UISlotMediumMove.wav", SOUND_EFFECT, 0.3);

        if (m_iCurIdx >= 10)
            return;
        m_iCurIdx += 5;
    }
    else if (m_iCurIdx < 0)
        return;

    if (Engine::GetKeyDown(DIK_UP))
    {
        Engine::Play_Sound(L"SFX_70_UISlotMediumMove.wav", SOUND_EFFECT, 0.3);

        m_iCurIdx -= 5;
        if (m_iCurIdx < 0)
            m_iCurIdx = -5;
    }
    if (Engine::GetKeyDown(DIK_LEFT))
    {
        Engine::Play_Sound(L"SFX_70_UISlotMediumMove.wav", SOUND_EFFECT, 0.3);

        if (m_iCurIdx % 5 == 0)
            return;
        --m_iCurIdx;
    }
    if (Engine::GetKeyDown(DIK_RIGHT))
    {
        Engine::Play_Sound(L"SFX_70_UISlotMediumMove.wav", SOUND_EFFECT, 0.3);

        if (m_iCurIdx % 5 == 4)
            return;
        ++m_iCurIdx;
    }
    if (Engine::GetKeyDown(DIK_D)) // 닫기
    {
        Engine::Play_Sound(L"SFX_120_UINormalClose.wav", SOUND_EFFECT, 0.3);

        m_iCurIdx = -5;
    }

    if (Engine::GetKeyDown(DIK_S)) // 아이템 구매
    {
        _int    iPrice = m_pItem->Get_ItemInfo().iPrice;
        if (m_pPlayer->GetPlayerCoin() >= iPrice)
        {
            Engine::Play_Sound(L"SFX_504_ItemDrop.wav", SOUND_EFFECT, 0.8f);
            
            m_pInven->Add_Item(m_pItem);
            m_pPlayer->SetPlayerCoin(-iPrice);
            return;
        }
        Engine::Play_Sound(L"SFX_306_UIFail.wav", SOUND_EFFECT, 0.8f);

        // ++플레이어에서 돈도 빼
    }

}

void CStoreSelector::Update_ItemInfo()
{
    if (m_iCurIdx < 0)
        return;

    _int	iItemCount = m_pStore->Get_ItemCount();

    if (iItemCount <= m_iCurIdx)
    {
        m_iCurIdx = m_iPrevIdx;
        return;
    }

    m_pItem = m_pStore->Get_Item(m_iCurIdx); // 현재 아이템 정보
    _vec3 vPos;
    vPos = m_pItem->Get_ItemPos(); // 아이템의 위치
    m_pTransformCom->m_vInfo[INFO_POS] = vPos; // 위치를 아이템의 위치로
    m_pTransformCom->m_vInfo[INFO_POS].x += 2.2f;
    m_tCopyInfo = m_pItem->Get_ItemInfo(); // 아이템 텍스트정보를 복사 

}

CStoreSelector* CStoreSelector::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CStoreSelector* pUI = new CStoreSelector(pGraphicDev);

    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("StoreSelector Create Failed");
        return nullptr;
    }

    return pUI;
}

void CStoreSelector::Free()
{
    Engine::CGameObject::Free();
}
