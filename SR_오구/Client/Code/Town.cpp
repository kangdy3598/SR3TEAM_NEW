#include "pch.h"
#include "Town.h"
#include "Export_Utility.h"

CTown::CTown(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
{
}

CTown::~CTown()
{
}

HRESULT CTown::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    return S_OK;
}

_int CTown::Update_GameObject(const float& fTimeDelta)
{
    Add_RenderGroup(RENDER_NONALPHA, this);

    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CTown::LateUpdate_GameObject(const float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTown::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CTown::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CMapTex*>(Engine::Clone_Proto(L"Proto_TownMapTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_TownMap"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture",pComponent });


    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });

    return S_OK;
}

HRESULT CTown::SetUp_Material()
{
    return S_OK;
}

CTown* CTown::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTown* pTown = new CTown(pGraphicDev);

    if (FAILED(pTown->Ready_GameObject()))
    {
        Safe_Release(pTown);
        MSG_BOX("pMap Create Failed");
        return nullptr;
    }

    return pTown;
}

void CTown::Free()
{
    Engine::CGameObject::Free();
}
