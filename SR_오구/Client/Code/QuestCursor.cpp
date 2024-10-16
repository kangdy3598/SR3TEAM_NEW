#include "pch.h"
#include "QuestCursor.h"

CQuestCursor::CQuestCursor(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
{
}

CQuestCursor::~CQuestCursor()
{
}

HRESULT CQuestCursor::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_vCursorPos = { -172.f, 60.f, 0.3f };
    m_fYInterval = { 105.f };
    return S_OK;
}

void CQuestCursor::LateReady_GameObject()
{
}

_int CQuestCursor::Update_GameObject(const _float& fTimeDelta)
{
    m_pQuestUI = dynamic_cast<CQuestUI*>(Engine::Get_GameObject(L"Layer_UI", L"Quest_UI"));
    NULL_CHECK_RETURN(m_pQuestUI,0);

    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    Key_Input(fTimeDelta);

    Update_QuestInfo();

    Engine::Add_RenderGroup(RENDER_UI, this);

    return iExit;
}

void CQuestCursor::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CQuestCursor::Render_GameObject()
{
    if (m_iCurIdx < 0)
        return;

    _vec2 QuestNamePos(930.f, 145.f);
    Engine::Render_Font(L"Font_Ogu24", m_tCopyInfo.pQuestTitle, &QuestNamePos, D3DXCOLOR(0.999f, 0.98f, 0.9f, 0.7f));

    _vec2 QuestContentPos(930.f, 195.f);
    _vec2 vXYSize(290.f, 400.f);
    Engine::Render_Font(L"Font_Ogu22", m_tCopyInfo.pQuestContent, &QuestContentPos, D3DXCOLOR(0.999f, 0.98f, 0.9f, 0.7f), TEXT_DEFAULT, vXYSize);
    
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pTextureCom->Set_Texture();
    m_pBufferCom->Render_Buffer();



}

HRESULT CQuestCursor::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_QuestCursor"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureCursor", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformCursor", pComponent });
    m_pTransformCom->m_vScale = { 377.5f, 54.f, 1.f }; 
    m_pTransformCom->m_vInfo[INFO_POS] = m_vCursorPos;

    return S_OK;
}

void CQuestCursor::Key_Input(const _float& fTimeDelta)
{
    m_iPrevIdx = m_iCurIdx;

    if (Engine::GetKeyDown(DIK_DOWN))
    {
        Engine::Play_Sound(L"SFX_70_UISlotMediumMove.wav", SOUND_EFFECT, 0.3);
        if (m_iCurIdx >= 3)
            return;
        m_iCurIdx += 1;
    }

    if (Engine::GetKeyDown(DIK_UP))
    {
        Engine::Play_Sound(L"SFX_70_UISlotMediumMove.wav", SOUND_EFFECT, 0.3);

        m_iCurIdx -= 1;
        if (m_iCurIdx < 0)
            m_iCurIdx = -1;
    }
    if (Engine::GetKeyDown(DIK_LEFT))
    {
        Engine::Play_Sound(L"SFX_70_UISlotMediumMove.wav", SOUND_EFFECT, 0.3);

        m_iCurIdx -= 1;
        if (m_iCurIdx < 0)
            m_iCurIdx = -1;
    }
    if (Engine::GetKeyDown(DIK_RIGHT))
    {
        Engine::Play_Sound(L"SFX_70_UISlotMediumMove.wav", SOUND_EFFECT, 0.3);

        if (m_iCurIdx >= 3)
            return;
        m_iCurIdx += 1;
    }
    if (Engine::GetKeyDown(DIK_O))
    {
        m_iCurIdx = -1;
    }

}

void CQuestCursor::Update_QuestInfo()
{
    if (m_iCurIdx < 0)
        return;

    _int	iQuestCount = m_pQuestUI->Get_QuestArray()->size(); //¹è¿­size();

    if (iQuestCount <= m_iCurIdx)
    {
        m_iCurIdx = m_iPrevIdx;
        return;
    }

    m_tCopyInfo = m_pQuestUI->Get_Quest(m_iCurIdx);

    _vec3 vResultPos = m_vCursorPos;
    vResultPos.y = m_vCursorPos.y - (m_fYInterval * m_iCurIdx);
    
    m_pTransformCom->m_vInfo[INFO_POS] = vResultPos;
}

CQuestCursor* CQuestCursor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CQuestCursor* pUI = new CQuestCursor(pGraphicDev);

    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("QuestCursor Create Failed");
        return nullptr;
    }

    return pUI;
}

void CQuestCursor::Free()
{
    Engine::CGameObject::Free();
}
