#include "pch.h"
#include "SpeedUI.h"

CSpeedUI::CSpeedUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBuffUI(pGraphicDev)
{
}

CSpeedUI::~CSpeedUI()
{
}

HRESULT CSpeedUI::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_fViewZ = 0.1;
    return S_OK;
}

void CSpeedUI::LateReady_GameObject()
{
}

_int CSpeedUI::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    if (m_fBuffTime > 0)
    {
        Update_SettingTime(fTimeDelta);
        Engine::Add_RenderGroup(RENDER_UI, this);
    }

    return iExit;
}

void CSpeedUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CSpeedUI::Render_GameObject()
{
    if (m_fBuffTime > 0)
    {
        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
        m_pTextureCom->Set_Texture();
        m_pBufferCom->Render_Buffer();

        _vec2 vCountPos;

        vCountPos.x = m_pTransformCom->m_vInfo[INFO_POS].x + (WINCX * 0.5f) - 28;
        vCountPos.y = -(m_pTransformCom->m_vInfo[INFO_POS].y) + (WINCY * 0.5f) + 30;

        wchar_t Division[32] = L":";
        wchar_t strBuffMinute[32];
        wchar_t strBuffSecond[32];

        _int fMinute = (int)m_fBuffTime / 60.f;
        _int fSecond = (int)m_fBuffTime % 60;

        swprintf(strBuffMinute, 32, L"%02d", fMinute); //분 추출
        swprintf(strBuffSecond, 32, L"%02d", fSecond); //초 추출

        wcscat_s(strBuffMinute, 32, Division);   // "분 + :"
        wcscat_s(strBuffMinute, 32, strBuffSecond);   // "분 : + 초"

        Engine::Render_Font(L"Font_Ogu24", strBuffMinute, &vCountPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

    }
}

HRESULT CSpeedUI::Add_Component()
{
    CComponent* pComponent = NULL;

    // 렉트텍스쳐
    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_SpeedUI"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vInfo[INFO_POS] = { -585.f, 90.f, 0.1f };
    m_pTransformCom->m_vScale = { 40.f, 40.f, 1.f };

    return S_OK;

}

CSpeedUI* CSpeedUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CSpeedUI* pUI = new CSpeedUI(pGraphicDev);

    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("SpeedUI Create Failed");
        return nullptr;
    }

    return pUI;
}

void CSpeedUI::Free()
{
}
