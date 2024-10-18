#include "pch.h"
#include "SeaLion.h"

CSeaLion::CSeaLion(LPDIRECT3DDEVICE9 pGraphicDev)
    :CStoreNPC(pGraphicDev)
{
}

CSeaLion::~CSeaLion()
{
}

HRESULT CSeaLion::Ready_GameObject()
{
    CStoreNPC::Ready_GameObject();

    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/SeaLion.png", &m_pTex);
    m_pAnimatorCom->CreateAnimation(L"SeaLion", m_pTex, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.12f, 7);

    m_tInfo.pName = L"꼬치 바다사자";
    m_tInfo.pContent = L"생선꼬치 마싯어요. 사주새요.";

    return S_OK;
}

void CSeaLion::LateReady_GameObject()
{
    CStoreNPC::LateReady_GameObject();
}

_int CSeaLion::Update_GameObject(const _float& fTimeDelta)
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

void CSeaLion::LateUpdate_GameObject(const _float& fTimeDelta)
{
    for (size_t i = 0; i < m_ItemArray.size(); i++)
    {
        m_ItemArray[i]->LateUpdate_GameObject(fTimeDelta);
    }
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CSeaLion::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    m_pGraphicDev->SetTexture(0, m_pTex);
    m_pAnimatorCom->Play(L"SeaLion", true);
    m_pAnimatorCom->render();
    ////m_pColliderCom->Render_Buffer();

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pShopTransformCom->Get_WorldMatrix());
    m_pTextureCom->Set_Texture();
    m_pBufferCom->Render_Buffer();
}

void CSeaLion::OnCollision(CGameObject* _pOther)
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

            CItem* pItem = dynamic_cast<CCookSmallFish*>(CCookSmallFish::Create(m_pGraphicDev));
            NULL_CHECK_RETURN(pItem);
            pItem->LateReady_GameObject();
            m_ItemArray.push_back(pItem);

            pItem = dynamic_cast<CCookMiddleFish*>(CCookMiddleFish::Create(m_pGraphicDev));
            NULL_CHECK_RETURN(pItem);
            pItem->LateReady_GameObject();
            m_ItemArray.push_back(pItem);

            pItem = dynamic_cast<CCookBigFish*>(CCookBigFish::Create(m_pGraphicDev));
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

void CSeaLion::OnCollisionEnter(CGameObject* _pOther)
{
}

void CSeaLion::OnCollisionExit(CGameObject* _pOther)
{
    m_pInterButton->CallButton(false); // 대화 중일 경우 버튼 출력이 필요 없음!!!!
}

HRESULT CSeaLion::Add_Component()
{
    CComponent* pComponent = NULL;

    // 렉트텍스쳐
    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_FishShop"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_TextureFishShop", pComponent });

    pComponent = m_pShopTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformFishShop", pComponent });
    m_pShopTransformCom->m_vScale = { 30.f, 30.f, 30.f };
    m_pShopTransformCom->Set_Pos(630.f, 30.f, 229.f);

    //-여기까지 텍스쳐-

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 20.f, 20.f, 30.f };
    m_pTransformCom->Set_Pos(630.f, 25.f, 230.f);

    pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_pColliderCom->SetGameObjectPtr(this);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

    return S_OK;
}

CSeaLion* CSeaLion::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CSeaLion* pNPC = new CSeaLion(pGraphicDev);

    if (FAILED(pNPC->Ready_GameObject()))
    {
        Safe_Release(pNPC);
        MSG_BOX("SeaLion Create Failed");
        return nullptr;
    }

    return pNPC;
}

void CSeaLion::Free()
{
    Engine::CGameObject::Free();
}
