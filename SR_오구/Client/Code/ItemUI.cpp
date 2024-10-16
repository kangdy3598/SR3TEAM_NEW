#include "pch.h"
#include "ItemUI.h"

CItemUI::CItemUI(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , m_bCall(false)
    , m_pPlayer(nullptr)
{
}

CItemUI::~CItemUI()
{
}

HRESULT CItemUI::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_fViewZ = 0.9f;
    return S_OK;
}

_int CItemUI::Update_GameObject(const _float& fTimeDelta)
{

    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    if (m_bCall && m_pTextureCom)
    {
        Engine::Add_RenderGroup(RENDER_UI, this);
        Key_Input(fTimeDelta);
    }

    return iExit;
}

void CItemUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CItemUI::Render_GameObject()
{
    if (m_bCall)
    {
        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
        m_pTextureCom->Set_Texture();
        m_pBufferCom->Render_Buffer();

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pItemTransformCom->Get_WorldMatrix());
        m_pItemTextureCom->Set_Texture();
        m_pBufferCom->Render_Buffer();

        Render_Text();
    }
}

void CItemUI::Key_Input(const _float& fTimeDelta)
{
    if (m_bCall)
    {
        if (Engine::GetKeyDown(DIK_S))
        {
            //대화내용 리셋
            Engine::Play_Sound(L"SFX_68_UIBig_Close.wav", SOUND_EFFECT, 0.7);

            Engine::Get_Layer(L"Layer_GameLogic")->SetGameState(GAMESTATE_NONE);
            //dynamic_cast<CPlayer*>(_pOther)->ChangeIdleState();
            ZeroMemory(&m_tCopyInfo, 0, sizeof(m_tCopyInfo));
            m_bCall = false; // 대화창 부르지않음
            m_pTextureCom = nullptr;
        }
    }
}

void CItemUI::Render_Text()
{
    _vec2 vTitlePos(350.f, 180.f);
    _vec2 TitleSize(580.f, 80.f);

    Engine::Render_Font(L"Font_OguBold36", m_tCopyInfo.pName, &vTitlePos, D3DXCOLOR(0.999f, 0.98f, 0.9f, 1.f), TEXT_CENTER, TitleSize);

    _vec2 vInfoPos(350.f, 460.f);
    _vec2 InfoSize(580.f, 80.f);

    Engine::Render_Font(L"Font_Ogu24", m_tCopyInfo.pInfo, &vInfoPos, D3DXCOLOR(0.999f, 0.98f, 0.9f, 1.f), TEXT_CENTER, InfoSize);

}

HRESULT CItemUI::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_AcquiredItemUI"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture",pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 640.f, 360.f, 0.9f };
    m_pTransformCom->Set_Pos(0.f, 0.f, 1.f);

    pComponent = m_pItemTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_ItemTransform", pComponent });
    m_pItemTransformCom->m_vScale = { 80.f, 80.f, 0.2f };
    m_pItemTransformCom->Set_Pos(0.f, 0.f, 1.f);

    return S_OK;
}

CItemUI* CItemUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CItemUI* pItem = new CItemUI(pGraphicDev);

    if (FAILED(pItem->Ready_GameObject()))
    {
        Safe_Release(pItem);
        MSG_BOX("ItemUI Create Failed");
        return nullptr;
    }

    return pItem;
}

void CItemUI::Free()
{
    Engine::CGameObject::Free();
}

