#include "pch.h"
#include "QuestAccept.h"

CQuestAccept::CQuestAccept(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , m_bCall(false)
{
}

CQuestAccept::~CQuestAccept()
{
}

HRESULT CQuestAccept::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_fViewZ = 0.9f;
    return S_OK;
}

_int CQuestAccept::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    if (m_bCall)
    {
        Engine::Add_RenderGroup(RENDER_UI, this);
        Key_Input(fTimeDelta);
    }

    return iExit;
}

void CQuestAccept::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CQuestAccept::Render_GameObject()
{
    if (m_bCall)
    {
        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
        m_pTextureCom->Set_Texture();
        m_pBufferCom->Render_Buffer();

        Render_Text();
    }
}

void CQuestAccept::Key_Input(const _float& fTimeDelta)
{
    if (m_bCall)
    {
        if (Engine::GetKeyDown(DIK_S))
        {
            Engine::Play_Sound(L"SFX_120_UINormalClose.wav", SOUND_EFFECT, 0.8);

            //대화내용 리셋
            ZeroMemory(&m_tCopyInfo, 0, sizeof(m_tCopyInfo));
            m_bCall = false; // 대화창 부르지않음
        }
    }
}

void CQuestAccept::Render_Text()
{
    // 이름 텍스트 렌더링 >> NPC 이름
    _vec2 vTitlePos(360.f, 324.f);

    Engine::Render_Font(L"Font_Ogu24", m_tCopyInfo.pQuestTitle, &vTitlePos, D3DXCOLOR(0.999f, 0.98f, 0.9f, 1.f), TEXT_DEFAULT);

    _vec2 vStartPos = { 320, 382 };  // 글자담을 렉트 LT!!!
    _vec2 RCSize2(675, 100); // RB^^ㅋㅋ

    Engine::Render_Font(L"Font_Ogu24", m_tCopyInfo.pQuestContent, &vStartPos, D3DXCOLOR(0.999f, 0.98f, 0.9f, 1.f), TEXT_CENTER, RCSize2);

}

HRESULT CQuestAccept::Add_Component()
{
    CComponent* pComponent = NULL;

    // 렉트텍스쳐
    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    //QuestAcceptUI
    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_QuestAcceptUI"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_TextureAcceptUI", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformAcceptUI", pComponent });
    m_pTransformCom->m_vInfo[INFO_POS] = { 0.f, 0.f, 0.9f };
    m_pTransformCom->m_vScale = { 640.f, 360.f, 1.f };

    return S_OK;
}

CQuestAccept* CQuestAccept::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CQuestAccept* pUI = new CQuestAccept(pGraphicDev);

    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("TextBox Create Failed");
        return nullptr;
    }
    return pUI;
}

void CQuestAccept::Free()
{
    Engine::CGameObject::Free();
}
