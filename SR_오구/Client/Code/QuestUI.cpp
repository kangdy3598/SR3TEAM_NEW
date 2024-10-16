#include "pch.h"
#include "QuestUI.h"
#include "Player.h"
#include "QuestCursor.h"

CQuestUI::CQuestUI(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
{
}

CQuestUI::~CQuestUI()
{
}

HRESULT CQuestUI::Ready_GameObject()
{
    m_bDontDestroy = true;
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_fViewZ = 0.9f;
    m_vQuestInterval = { 80.f, 105.f };
    m_vQuestPos = {195.f, 286.f};

    //위치 세팅 테스트 코드 
    //QUEST_INFO dlatl = { L"안녕바리", L"애브리바디" };
    //for (size_t i = 0; i < 3; i++)
    //{
    //    m_QuestArray.push_back(dlatl);
    //}

    return S_OK;
}

void CQuestUI::LateReady_GameObject()
{
    Engine::Add_RenderGroup(RENDER_UI, this); // 이거 넣어야 다음 씬에도 들어가지더라..

    m_pCursor = dynamic_cast<CQuestCursor*>(CQuestCursor::Create(m_pGraphicDev));
    NULL_CHECK_RETURN(m_pCursor);

    m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
    NULL_CHECK_RETURN(m_pPlayer);
}

_int CQuestUI::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    if (m_pPlayer->GetPlayerQuestUI())
    {
        Engine::Add_RenderGroup(RENDER_UI, this);
        //Key_Input(fTimeDelta);
        if (m_pCursor)
        {
            m_pCursor->Update_GameObject(fTimeDelta);
            m_pCursor->LateUpdate_GameObject(fTimeDelta);
        }
    }

    return iExit;

}

void CQuestUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CQuestUI::Render_GameObject()
{
    if (m_pPlayer->GetPlayerQuestUI())
    {
        _vec2 HpPosition(580.f, 15.f);
        Engine::Render_Font(L"Font_OguBold48", L"퀘스트", &HpPosition, D3DXCOLOR(0.999f, 0.98f, 0.9f, 1.f));

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
        m_pTextureCom->Set_Texture();
        m_pBufferCom->Render_Buffer();

        for (_int i = 0; i < m_QuestArray.size(); i++)
        {
            _vec2 TextPos = m_vQuestPos;
            TextPos.y = m_vQuestPos.y + (m_vQuestInterval.y * i);
            Engine::Render_Font(L"Font_Ogu24", m_QuestArray[i].pQuestTitle, &TextPos, D3DXCOLOR(0.999f, 0.98f, 0.9f, 1.f));
        }

        if (m_pCursor)
        {
            m_pCursor->Render_GameObject();
        }
        
    }
}

HRESULT CQuestUI::Add_Component()
{
    CComponent* pComponent = NULL;

    // 렉트텍스쳐
    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    //QuestUI
    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_QuestUI"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vInfo[INFO_POS] = { 0.f, 0.f, 0.5f };
    m_pTransformCom->m_vScale = { 640.f, 360.f, 1.f };

    return S_OK;
}

CQuestUI* CQuestUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CQuestUI* pUI = new CQuestUI(pGraphicDev);

    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("InvenUI Create Failed");
        return nullptr;
    }

    return pUI;
}

void CQuestUI::Free()
{
    Engine::CGameObject::Free();
}
