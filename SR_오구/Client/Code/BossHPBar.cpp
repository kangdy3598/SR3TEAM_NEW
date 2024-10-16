#include "pch.h"
#include "BossHPBar.h"

CBossHPBar::CBossHPBar(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
{
}

CBossHPBar::~CBossHPBar()
{
}

HRESULT CBossHPBar::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    return S_OK;
}

_int CBossHPBar::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    Engine::Add_RenderGroup(RENDER_UI, this);

    return iExit;
}

void CBossHPBar::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    m_fViewZ = 0.1f;
}

void CBossHPBar::Render_GameObject()
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

    //0915 추가 코드 HPBarSize
    float fHpSizeX = 275.5f; //HPSIZE
    float fHpPosX = 0.f; //HP바의 X좌표 위치
    float fHpBarSize = (fHpSizeX / m_pInfo->iMaxHP);// 피 1당 Hpbar사이즈
    
    m_pTransformCom[HPBAR]->m_vScale.x = fHpBarSize * m_pInfo->iCurHP; // 피 1 사이즈 * 현재 HP 
    m_pTransformCom[HPBAR]->m_vInfo[INFO_POS].x = fHpPosX - (fHpBarSize * (m_pInfo->iMaxHP - m_pInfo->iCurHP));
    // 중점이 0이기 때문에 가운데 중심으로 줄어들어서 위치도 피 1사이즈 만큼씩 줄여준당. ㅈㄴ상냥하게 다적음^__^보면 칭찬 부틱

    _vec2 vNamePos = { 0.f,0.f };
    _vec2 vEndPos = { 1280.f, 80.f };
    Engine::Render_Font(L"Font_OguBold36", m_pInfo->pBossName, &vNamePos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), TEXT_CENTER, vEndPos);

}

HRESULT CBossHPBar::Add_Component()
{
    CComponent* pComponent = NULL;

    // 렉트텍스쳐
    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });


    //HPFRAME
    pComponent = m_pTextureCom[HPFRAME] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossHPFrame"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_TextureFrame", pComponent });

    pComponent = m_pTransformCom[HPFRAME] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformFrame", pComponent });
    m_pTransformCom[HPFRAME]->m_vInfo[INFO_POS] = { 0.f, 278.5f, 0.1f }; // UI 위치긴한디 상수로 넣어도 됨?ㅇㅅㅇ
    m_pTransformCom[HPFRAME]->m_vScale = { 275.5f, 11.f, 1.f }; // 해당 UI의 스케일


    //HPBAR
    pComponent = m_pTextureCom[HPBAR] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossHPBar"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TextureHPBar", pComponent });

    pComponent = m_pTransformCom[HPBAR] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformHPBar", pComponent });
    m_pTransformCom[HPBAR]->m_vInfo[INFO_POS] = { 0.f, 278.5f, 0.1f };
    m_pTransformCom[HPBAR]->m_vScale = { 275.5f, 11.f, 1.f };


    //HPBACK
    pComponent = m_pTextureCom[HPBACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossHPBack"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_TextureHPBack", pComponent });

    pComponent = m_pTransformCom[HPBACK] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_TransformHPBack", pComponent });
    m_pTransformCom[HPBACK]->m_vInfo[INFO_POS] = { 0.f, 278.5f, 0.1f };
    m_pTransformCom[HPBACK]->m_vScale = { 275.5f, 11.f, 1.f };

    return S_OK;

}

CBossHPBar* CBossHPBar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBossHPBar* pUI = new CBossHPBar(pGraphicDev);

    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("UI Create Failed");
        return nullptr;
    }

    return pUI;
}

void CBossHPBar::Free()
{
    Engine::CGameObject::Free();
}
