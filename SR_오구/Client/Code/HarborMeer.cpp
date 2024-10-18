#include "pch.h"
#include "HarborMeer.h"

CHarborMeer::CHarborMeer(LPDIRECT3DDEVICE9 pGraphicDev)
    :CStoreNPC(pGraphicDev)
{
}

CHarborMeer::~CHarborMeer()
{
}

HRESULT CHarborMeer::Ready_GameObject()
{
    CStoreNPC::Ready_GameObject();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/MeerCat.png", &m_pTex);
    m_pAnimatorCom->CreateAnimation(L"MeerCat", m_pTex, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.12f, 3);

    m_tInfo.pName = L"항구 미어캣";
    m_tInfo.pContent = L"강해지는 뿌리를 팔고있어요..";

    return S_OK;
}

void CHarborMeer::LateReady_GameObject()
{
    CStoreNPC::LateReady_GameObject();
}

_int CHarborMeer::Update_GameObject(const _float& fTimeDelta)
{
    CStoreNPC::Update_GameObject(fTimeDelta);

    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    for (size_t i = 0; i < m_ItemArray.size(); i++)
    {

        m_ItemArray[i]->Update_GameObject(fTimeDelta);
    }

    Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CHarborMeer::LateUpdate_GameObject(const _float& fTimeDelta)
{
    for (size_t i = 0; i < m_ItemArray.size(); i++)
    {
        m_ItemArray[i]->LateUpdate_GameObject(fTimeDelta);
    }
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CHarborMeer::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    m_pGraphicDev->SetTexture(0, m_pTex);
    m_pAnimatorCom->Play(L"MeerCat", true);
    m_pAnimatorCom->render();
    ////m_pColliderCom->Render_Buffer();

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pShopTransformCom->Get_WorldMatrix());
    m_pTextureCom->Set_Texture();
    m_pBufferCom->Render_Buffer();

}

void CHarborMeer::OnCollision(CGameObject* _pOther)
{
    if (_pOther->GetObjectKey() != L"Player")
        return;

    if (Engine::GetKeyDown(DIK_D))
    {
        m_ItemArray.clear();
        m_bStoreOn = false;
        m_pInterButton->CallButton(true); // 대화중이 아닐 때 버튼 활성화
    }
    else if (m_bStoreOn)
    {
        return;
    }

    if (Engine::GetKeyDown(DIK_S))
    {
        m_bConversation = m_bConversation ? false : true;
        // 대화중일때 S를 누르면 대화종료
        // 대화중이 아닐때 S를 누르면 대화시작
        if (!m_bConversation)
        {
            // 여기에 UIbool true + 아이템 세팅
            m_bStoreOn = true;

            CItem* pItem = dynamic_cast<CSmallRoot*>(CSmallRoot::Create(m_pGraphicDev));
            NULL_CHECK_RETURN(pItem);
            pItem->LateReady_GameObject();
            m_ItemArray.push_back(pItem);

            pItem = dynamic_cast<CMiddleRoot*>(CMiddleRoot::Create(m_pGraphicDev));
            NULL_CHECK_RETURN(pItem);
            pItem->LateReady_GameObject();
            m_ItemArray.push_back(pItem);

            pItem = dynamic_cast<CBigRoot*>(CBigRoot::Create(m_pGraphicDev));
            NULL_CHECK_RETURN(pItem);
            pItem->LateReady_GameObject();
            m_ItemArray.push_back(pItem);

            for (size_t i = 0; i < m_ItemArray.size(); i++)
            {
                m_pStoreUI->Add_Item(m_ItemArray[i]);
            }
            m_pStoreUI->CallStoreUI(true);

            return;
        }

        if (m_bConversation)
        {
            m_pInterButton->CallButton(false); // 대화 중일 경우 버튼 출력이 필요 없음!!!!

            m_pTextBox->Set_Text(m_tInfo); //대화창 텍스트 세팅
            m_pTextBox->CallTextBox(true); //대화창 호출
        }

    }
    if (!m_bConversation)
    {
        m_pInterButton->CallButton(true); // 대화중이 아닐 때 버튼 활성화
        // 대화하기[S]
    }

}

void CHarborMeer::OnCollisionEnter(CGameObject* _pOther)
{
}

void CHarborMeer::OnCollisionExit(CGameObject* _pOther)
{
    m_pInterButton->CallButton(false); // 대화 중일 경우 버튼 출력이 필요 없음!!!!
}

HRESULT CHarborMeer::Add_Component()
{
    CComponent* pComponent = NULL;

    // 렉트텍스쳐
    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_ConsumMarket"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pShopTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformHatShop", pComponent });
    m_pShopTransformCom->m_vScale = { 100.f, 50.f, 30.f };
    m_pShopTransformCom->Set_Pos(520.f, 45.f, 910.f);

    //-여기까지 텍스쳐-

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 30.f, 30.f, 30.f };
    m_pTransformCom->Set_Pos(540.5f, 20.f, 911.f);

    pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_pColliderCom->SetGameObjectPtr(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

    return S_OK;

}

CHarborMeer* CHarborMeer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CHarborMeer* pNPC = new CHarborMeer(pGraphicDev);

    if (FAILED(pNPC->Ready_GameObject()))
    {
        Safe_Release(pNPC);
        MSG_BOX("CHarborMeer Create Failed");
        return nullptr;
    }

    return pNPC;
}

void CHarborMeer::Free()
{
    Engine::CGameObject::Free();
}
