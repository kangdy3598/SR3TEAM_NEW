#include "pch.h"
#include "TextBox.h"

CTextBox::CTextBox(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , m_bCall(false)
    , m_iCurStrIndex(1)
{
    ZeroMemory(&m_tCopyInfo, 0, sizeof(m_tCopyInfo));
}

CTextBox::~CTextBox()
{
}

HRESULT CTextBox::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    return S_OK;
}

_int CTextBox::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    if (m_bCall)
    {
        Engine::Add_RenderGroup(RENDER_UI, this);
        Key_Input(fTimeDelta);
    }

    return iExit;
}

void CTextBox::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTextBox::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pTextureCom->Set_Texture();
    m_pBufferCom->Render_Buffer();



    if (m_bCall)
        Render_Text();
}

void CTextBox::Key_Input(const _float& fTimeDelta)
{
    if (m_bCall)
    {
        if (Engine::GetKeyDown(DIK_S))
        {
            //대화내용 리셋
            Engine::Play_Sound(L"SFX_70_UISlotMediumMove.wav", SOUND_EFFECT, 0.3);
            ZeroMemory(&m_tCopyInfo, 0, sizeof(m_tCopyInfo));
            m_bCall = false; // 대화창 부르지않음
            m_iCurStrIndex = 1; // 초기화
        }
    }
}

void CTextBox::Render_Text()
{
    // 이름 텍스트 렌더링 >> NPC 이름 
    _vec2 vNPCNamePos(221, 494);
    _vec2 vNameRCSize(266, 74);

    Engine::Render_Font(L"Font_Ogu24", m_tCopyInfo.pName, &vNPCNamePos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), TEXT_CENTER, vNameRCSize);


    _int iTextLength = wcslen(m_tCopyInfo.pContent);  // 전체 문자열 길이

    if (m_iCurStrIndex > iTextLength)
        m_iCurStrIndex = iTextLength;

    wstring strPartialText(m_tCopyInfo.pContent, m_iCurStrIndex);

    _vec2 vNPCConverPos = { 405, 570 };  // 글자담을 렉트 LT!!!
    _vec2 vConverRCSize(500, 100); // RB^^ㅋㅋ

    Engine::Render_Font(L"Font_Ogu24", strPartialText.c_str(), &vNPCConverPos, D3DXCOLOR(0.999f, 0.98f, 0.9f, 1.f), TEXT_CENTER, vConverRCSize);

    m_iCurStrIndex += 1;

}


HRESULT CTextBox::Add_Component()
{
    CComponent* pComponent = NULL;

    // 렉트텍스쳐
    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    //TextBox
    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_TextBox"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vInfo[INFO_POS] = { 0.f, -245.f, 0.9f };
    m_pTransformCom->m_vScale = { 450.f, 115.f, 1.f };

    return S_OK;
}

CTextBox* CTextBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTextBox* pUI = new CTextBox(pGraphicDev);

    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("TextBox Create Failed");
        return nullptr;
    }
    return pUI;
}

void CTextBox::Free()
{
    Engine::CGameObject::Free();
}
