#include "pch.h"
#include "InvenUI.h"

#include "Player.h"
#include "ItemSelector.h"

//vector<CItem*>	CInvenUI::m_ItemList[CItem::TYPE_END];
//D3DXVECTOR3 CInvenUI::m_vInvenPos[CItem::TYPE_END];

CInvenUI::CInvenUI(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , m_pItemSelector(nullptr)
{
}

CInvenUI::~CInvenUI()
{
}

HRESULT CInvenUI::Ready_GameObject()
{
    m_bDontDestroy = true;
    
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    // 인벤토리 간격
    m_InvenInterval = { 145.7f, 137.f };
    m_iItemFilter = CItem::EQUIP;
    m_fViewZ = 0.9f;

    return S_OK;

}

void CInvenUI::LateReady_GameObject()
{
    Engine::Add_RenderGroup(RENDER_UI, this); // 이거 넣어야 다음 씬에도 들어가지더라..

    m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
    NULL_CHECK_RETURN(m_pPlayer);

    m_pItemSelector = dynamic_cast<CItemSelector*>(CItemSelector::Create(m_pGraphicDev));
    NULL_CHECK_RETURN(m_pItemSelector);

}

_int CInvenUI::Update_GameObject(const _float& fTimeDelta)
{
    Remove_Item();

    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    if (m_pPlayer->GetPlayerInven())
    {
        Engine::Add_RenderGroup(RENDER_UI, this);
        Key_Input(fTimeDelta);

        for (auto& pItem : m_ItemList[m_iItemFilter])
        {
            if (pItem == nullptr)
                continue;
            pItem->Update_GameObject(fTimeDelta);
        }

        if (m_pItemSelector)
        {
            m_pItemSelector->Update_GameObject(fTimeDelta);
        }
    }

    return iExit;
}

void CInvenUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    for (auto& pItem : m_ItemList[m_iItemFilter])
    {
        if (pItem == nullptr)
            continue;
        pItem->LateUpdate_GameObject(fTimeDelta);
    }
    if (m_pItemSelector)
    {
        m_pItemSelector->LateUpdate_GameObject(fTimeDelta);
    }
}

void CInvenUI::Render_GameObject()
{
    if (m_pPlayer->GetPlayerInven())
    {
        _vec2 HpPosition(556.f, 15.f);

        Engine::Render_Font(L"Font_OguBold48", L"인벤토리", &HpPosition, D3DXCOLOR(0.999f, 0.98f, 0.9f, 1.f));

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[BACKGROUND]->Get_WorldMatrix());
        m_pTextureCom[BACKGROUND]->Set_Texture();
        m_pBufferCom->Render_Buffer();

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[DEFAULT]->Get_WorldMatrix());
        m_pTextureCom[DEFAULT]->Set_Texture();
        m_pBufferCom->Render_Buffer();

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[FRAME]->Get_WorldMatrix());
        m_pTextureCom[FRAME]->Set_Texture();
        m_pBufferCom->Render_Buffer();

        // 아이콘에 알파값이 들어가야 함 -> 나중에 해야지 ㅋㅋ
        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[EQUIP]->Get_WorldMatrix());
        m_pTextureCom[EQUIP]->Set_Texture();
        m_pBufferCom->Render_Buffer();

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[CONSUM]->Get_WorldMatrix());
        m_pTextureCom[CONSUM]->Set_Texture();
        m_pBufferCom->Render_Buffer();

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[OTHER]->Get_WorldMatrix());
        m_pTextureCom[OTHER]->Set_Texture();
        m_pBufferCom->Render_Buffer();

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[QUEST]->Get_WorldMatrix());
        m_pTextureCom[QUEST]->Set_Texture();
        m_pBufferCom->Render_Buffer();

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[ICONBUTTON]->Get_WorldMatrix());
        m_pTextureCom[ICONBUTTON]->Set_Texture();
        m_pBufferCom->Render_Buffer();

        // 인벤토리 아이템 출력

        for (auto& pItem : m_ItemList[m_iItemFilter])
        {
            if (pItem == nullptr)
                continue;

            pItem->Render_GameObject();
        }
        if (m_pItemSelector)
        {
            m_pItemSelector->Render_GameObject();
        }

    }
}

_bool CInvenUI::Find_Item(CItem::ITEMTYPE _eType, _int _eItemEnum)
{
    if (m_ItemList[_eType].size() == 0)
        return false;

    for (auto& Item : m_ItemList[_eType])
    {
        if (Item->Get_ItemInfo().eItemEnum == _eItemEnum)
            return true;
    }
    return false;
}

void CInvenUI::Use_Efficacy(_int _iFilter, _int _iIdx)
{
    m_ItemList[_iFilter].at(_iIdx)->Use_Item();
}

void CInvenUI::Add_Item(CItem* _Item)
{
    _Item->Set_ItemDrop(false);
    _Item->Set_ItemScale(_vec3{ 55.f, 55.f, 0.1f });

    CItem::ITEMTYPE eType = _Item->Get_ItemInfo().eType; // 아이템 종류
    if (m_ItemList[eType].size() > 14) // 인벤 사이즈가 15이상이라면 리턴
        return;

    if (eType == CItem::CONSUM || eType == CItem::OTHER)// 소비기타일 경우 
    {
        _int eItemEnum = _Item->Get_ItemInfo().eItemEnum; // 들어온 아이템 이름
        for (size_t i = 0; i < m_ItemList[eType].size(); i++)
        {
            if (m_ItemList[eType].at(i)->Get_ItemInfo().eItemEnum == eItemEnum)//같은 아이템일경우
            {
                m_ItemList[eType].at(i)->Set_ItemCount(1);// 카운트를 +1해줌
                return;
            }
        }
    }

    m_ItemList[eType].push_back(_Item); // 해당 필터 벡터 뒤에 넣어줌
    Set_InvenPos(m_ItemList[eType].size() - 1, eType);
    _Item->Set_ItemPos(m_vInvenPos[eType]); //인벤 위치를 저장해둔 

}

void CInvenUI::Remove_Item()
{
    //아이템 0개되면 삭제치기
    for (size_t iFilter = 0; iFilter < CItem::TYPE_END; iFilter++)
    {
        vector<CItem*>::iterator iter = m_ItemList[iFilter].begin();
        while (iter != m_ItemList[iFilter].end())
        {
            if ((*iter)->Get_ItemInfo().iItemCount == 0)
            {
                // 삭제된 다음 요소
                iter = m_ItemList[iFilter].erase(iter);
            }
            else
            {
                ++iter;
            }
        }
        for (size_t j = 0; j < m_ItemList[iFilter].size(); j++)
        {
            // 인벤토리 위치를 업데이트
            Set_InvenPos(j, static_cast<CItem::ITEMTYPE>(iFilter));
            // 아이템의 위치를 새로 설정
            m_ItemList[iFilter][j]->Set_ItemPos(m_vInvenPos[iFilter]);
        }
    }

}

HRESULT CInvenUI::Add_Component()
{
    CComponent* pComponent = NULL;

    // 렉트텍스쳐
    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    //Background
    pComponent = m_pTextureCom[BACKGROUND] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_InvenBack"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureInvenBack", pComponent });

    pComponent = m_pTransformCom[BACKGROUND] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformInvenBack", pComponent });
    m_pTransformCom[BACKGROUND]->m_vInfo[INFO_POS] = { 0.f, 0.f, 0.9f };
    m_pTransformCom[BACKGROUND]->m_vScale = { 640.f, 360.f, 1.f };

    //InvenDefault
    pComponent = m_pTextureCom[DEFAULT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_InvenDefault"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureInvenDefault", pComponent });

    pComponent = m_pTransformCom[DEFAULT] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformInvenDefault", pComponent });
    m_pTransformCom[DEFAULT]->m_vInfo[INFO_POS] = { 0.f, 0.f, 0.2f };
    m_pTransformCom[DEFAULT]->m_vScale = { 640.f, 360.f, 1.f };

    //InvenFrame
    pComponent = m_pTextureCom[FRAME] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_InvenFrame"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureInvenFrame", pComponent });

    pComponent = m_pTransformCom[FRAME] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformInvenFrame", pComponent });
    m_pTransformCom[FRAME]->m_vInfo[INFO_POS] = { -165.f, -100.f, 0.2f };
    m_pTransformCom[FRAME]->m_vScale = { 382.f, 222.f, 1.f };

    //InvenIcon-Equip
    pComponent = m_pTextureCom[EQUIP] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_EquipIcon"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureInvenFrame", pComponent });

    pComponent = m_pTransformCom[EQUIP] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformEquipIcon", pComponent });
    m_pTransformCom[EQUIP]->m_vInfo[INFO_POS] = { -470.f, 165.f, 0.2f };
    m_pTransformCom[EQUIP]->m_vScale = { 50.f, 50.f, 1.f };

    //InvenIcon-Consum
    pComponent = m_pTextureCom[CONSUM] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ConsumIcon"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureConsumIcon", pComponent });

    pComponent = m_pTransformCom[CONSUM] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformConsumIcon", pComponent });
    m_pTransformCom[CONSUM]->m_vInfo[INFO_POS] = { -405.f, 165.f, 0.2f };
    m_pTransformCom[CONSUM]->m_vScale = { 50.f, 50.f, 1.f };

    //InvenIcon-Other
    pComponent = m_pTextureCom[OTHER] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_OtherIcon"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureOtherIcon", pComponent });

    pComponent = m_pTransformCom[OTHER] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformOtherIcon", pComponent });
    m_pTransformCom[OTHER]->m_vInfo[INFO_POS] = { -340.f, 165.f, 0.2f };
    m_pTransformCom[OTHER]->m_vScale = { 50.f, 50.f, 1.f };

    //InvenIcon-Quest
    pComponent = m_pTextureCom[QUEST] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_QuestIcon"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureQuestIcon", pComponent });

    pComponent = m_pTransformCom[QUEST] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformQuestIcon", pComponent });
    m_pTransformCom[QUEST]->m_vInfo[INFO_POS] = { -275.f, 165.f, 0.2f };
    m_pTransformCom[QUEST]->m_vScale = { 50.f, 50.f, 1.f };

    //ICONBUTTON
    pComponent = m_pTextureCom[ICONBUTTON] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_IconButton"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureIconButton", pComponent });

    pComponent = m_pTransformCom[ICONBUTTON] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformIconButton", pComponent });
    m_pTransformCom[ICONBUTTON]->m_vInfo[INFO_POS] = { -373.f, 158.f, 0.2f };
    m_pTransformCom[ICONBUTTON]->m_vScale = { 160.f, 13.f, 1.f };

    return S_OK;
}

void CInvenUI::Key_Input(const _float& fTimeDelta)
{

    if (Engine::GetKeyDown(DIK_E))
    {
        Engine::Play_Sound(L"SFX_70_UISlotMediumMove.wav", SOUND_EFFECT, 0.3);
        if (m_iItemFilter < 3)
        {
            ++m_iItemFilter;
            m_pItemSelector->ResetIdx();
        }
    }
    if (Engine::GetKeyDown(DIK_Q))
    {
        Engine::Play_Sound(L"SFX_70_UISlotMediumMove.wav", SOUND_EFFECT, 0.3);
        if (m_iItemFilter > 0)
        {
            --m_iItemFilter;
            m_pItemSelector->ResetIdx();
        }
    }

}

CInvenUI* CInvenUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CInvenUI* pUI = new CInvenUI(pGraphicDev);

    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("InvenUI Create Failed");
        return nullptr;
    }

    return pUI;
}

void CInvenUI::Free()
{
    Engine::CGameObject::Free();
}
