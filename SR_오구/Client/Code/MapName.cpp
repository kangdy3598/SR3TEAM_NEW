#include "pch.h"
#include "MapName.h"

CMapName::CMapName(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , m_bCall(false), m_fElapsedTime(0.f), m_SettingTime(2.f)
{
}

CMapName::~CMapName()
{
}

HRESULT CMapName::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    return S_OK;
}

_int CMapName::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_bCall)
        return 0;

    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    Engine::Add_RenderGroup(RENDER_UI, this);
    m_fElapsedTime += fTimeDelta;

    if (m_fElapsedTime >= m_SettingTime)
        m_bCall = false;

    return iExit;
}

void CMapName::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMapName::Render_GameObject()
{
    if (!m_bCall)
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pTextureCom->Set_Texture();
    m_pBufferCom->Render_Buffer();

    _vec2 LTPos(450.f, 30.f);
    _vec2 RBPos(385.f, 100.f);

    Engine::Render_Font(L"Font_OguBold36", m_pName, &LTPos, D3DXCOLOR(1.f, 1.f, 1.f, 0.7f), TEXT_CENTER, RBPos);

}



HRESULT CMapName::Add_Component()
{
    CComponent* pComponent = NULL;

    // 렉트텍스쳐
    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    //TextBox
    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MapName"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vInfo[INFO_POS] = { 0.f, 280.f, 0.1f };
    m_pTransformCom->m_vScale = { 200.f, 30.f, 1.f };

    return S_OK;

}

CMapName* CMapName::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMapName* pStone = new CMapName(pGraphicDev);

    if (FAILED(pStone->Ready_GameObject()))
    {
        Safe_Release(pStone);
        MSG_BOX("CMapName Create Failed");
        return nullptr;
    }

    return pStone;
}

void CMapName::Free()
{
    Engine::CGameObject::Free();
}
