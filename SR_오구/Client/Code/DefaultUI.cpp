#include "pch.h"
#include "DefaultUI.h"

CDefaultUI::CDefaultUI(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
{
}

CDefaultUI::~CDefaultUI()
{
}

HRESULT CDefaultUI::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    //0913 레이트 레디로 보내야 함 일단 업데이트로 보내놨다
    //m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
    //NULL_CHECK_RETURN(m_pPlayer, E_FAIL);

    return S_OK;

}

_int CDefaultUI::Update_GameObject(const _float& fTimeDelta)
{
    m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
    NULL_CHECK_RETURN(m_pPlayer, 0);

    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    Engine::Add_RenderGroup(RENDER_UI, this);

    return iExit;
}

void CDefaultUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    m_fViewZ = 0.3f;

}

void CDefaultUI::Render_GameObject()
{

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[HPBACK]->Get_WorldMatrix());
    m_pTextureCom[HPBACK]->Set_Texture();
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[HPBAR]->Get_WorldMatrix());
    m_pTextureCom[HPBAR]->Set_Texture();
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[HPFRAME]->Get_WorldMatrix());
    m_pTextureCom[HPFRAME]->Set_Texture();
    m_pBufferCom->Render_Buffer();

    //0913 HP 텍스트 렌더링
    //===========================
    CPlayer::PLAYERHP PlayerHP = m_pPlayer->GetPlayerHP();
    float PlayerCurHP = PlayerHP.iCurHP;
    float PlayerMaxHP = PlayerHP.iMaxHP;

    wchar_t Division[32] = L" / ";
    wchar_t HPStr[32];
    wchar_t MaxHPStr[32];

    swprintf(HPStr, 32, L"%d", (int)PlayerCurHP);
    swprintf(MaxHPStr, 32, L"%d", (int)PlayerMaxHP);

    wcscat_s(HPStr, 32, Division);   // "현재 HP + ' / '"
    wcscat_s(HPStr, 32, MaxHPStr);   // "현재 HP + ' / ' + 최대 HP"

    _vec2 HpPosition(175, 36);


    //Engine::Render_Font(L"Font_OguBold36", CoinStr, &position, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
    Engine::Render_Font(L"Font_OguBold36", HPStr, &HpPosition, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

    //0915 추가 코드 HPBarSize
    float fHpSizeX = 130.5f;
    float fHpPosX = -420.f;
    float fHpBarSize = (fHpSizeX / PlayerMaxHP);

    m_pTransformCom[HPBAR]->m_vScale.x = fHpBarSize * PlayerCurHP;
    m_pTransformCom[HPBAR]->m_vInfo[INFO_POS].x = fHpPosX - (fHpBarSize * (PlayerMaxHP - PlayerCurHP));

    //===========================

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[COIN_UI]->Get_WorldMatrix());
    m_pTextureCom[COIN_UI]->Set_Texture();
    m_pBufferCom->Render_Buffer();

    //0913 코인 텍스트 렌더링
    //===========================
    _int PlayerCoin = m_pPlayer->GetPlayerCoin();

    wchar_t CoinStr[100];
    swprintf(CoinStr, 100, L"%d", PlayerCoin);
    _vec2 CoinPosition(1115, 40);
    //Engine::Render_Font(L"Font_Default", (출력할 문자열 / 연동 플레이어 피통 & 코인), &position, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
    Engine::Render_Font(L"Font_OguBold36", CoinStr, &CoinPosition, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

    //===========================

}

HRESULT CDefaultUI::Add_Component()
{

    CComponent* pComponent = NULL;

    // 렉트텍스쳐
    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });


    //HPFRAME
    pComponent = m_pTextureCom[HPFRAME] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_HPFrame"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureFrame", pComponent });

    pComponent = m_pTransformCom[HPFRAME] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformFrame", pComponent });
    m_pTransformCom[HPFRAME]->m_vInfo[INFO_POS] = { -450.f, 310.f, 0.1f }; // UI 위치긴한디 상수로 넣어도 됨?ㅇㅅㅇ
    m_pTransformCom[HPFRAME]->m_vScale = { 174.f, 30.f, 1.f }; // 해당 UI의 스케일


    //HPBAR
    pComponent = m_pTextureCom[HPBAR] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_HPBar"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureHPBar", pComponent });

    pComponent = m_pTransformCom[HPBAR] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformHPBar", pComponent });
    m_pTransformCom[HPBAR]->m_vInfo[INFO_POS] = { -420.f, 310.f, 0.1f };
    m_pTransformCom[HPBAR]->m_vScale = { 130.5f, 17.f, 1.f };


    //HPBACK
    pComponent = m_pTextureCom[HPBACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_HPBarBack"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureHPBack", pComponent });

    pComponent = m_pTransformCom[HPBACK] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformHPBack", pComponent });
    m_pTransformCom[HPBACK]->m_vInfo[INFO_POS] = { -420.f, 310.f, 0.1f };
    m_pTransformCom[HPBACK]->m_vScale = { 134.f, 19.f, 1.f };


    // COINUI
    pComponent = m_pTextureCom[COIN_UI] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_CoinUI"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureHPBack", pComponent });

    pComponent = m_pTransformCom[COIN_UI] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformCoinUI", pComponent });
    m_pTransformCom[COIN_UI]->m_vInfo[INFO_POS] = { 510.f, 305.f, 0.1f };
    m_pTransformCom[COIN_UI]->m_vScale = { 104.f, 38.5f, 1.f };

    return S_OK;
}

CDefaultUI* CDefaultUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CDefaultUI* pUI = new CDefaultUI(pGraphicDev);

    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("UI Create Failed");
        return nullptr;
    }

    return pUI;
}

void CDefaultUI::Free()
{
    Engine::CGameObject::Free();
}
