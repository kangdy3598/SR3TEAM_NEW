#include "pch.h"
#include "BuffUI.h"

CBuffUI::CBuffUI(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev), m_fBuffTime(0)
{
}

CBuffUI::~CBuffUI()
{
}

void CBuffUI::Update_SettingTime(const _float& _fTimeDelta)
{
    if (0 > m_fBuffTime)
    {
        m_fBuffTime = 0;
    }
    m_fBuffTime -= _fTimeDelta;
}

HRESULT CBuffUI::Ready_GameObject()
{
    //FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    return S_OK;
}

void CBuffUI::LateReady_GameObject()
{
}

_int CBuffUI::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    return iExit;
}

void CBuffUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CBuffUI::Render_GameObject()
{
}

//HRESULT CBuffUI::Add_Component()
//{
//    return S_OK;
//}

void CBuffUI::Free()
{
    Engine::CGameObject::Free();
}
