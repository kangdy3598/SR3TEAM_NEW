#include "pch.h"
#include "Export_Utility.h"
#include "WorldHeartMap.h"




CWorldHeartMap::CWorldHeartMap(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
{
}

CWorldHeartMap::~CWorldHeartMap()
{
}

HRESULT CWorldHeartMap::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    return S_OK;
}

_int CWorldHeartMap::Update_GameObject(const float& fTimeDelta)
{

    Add_RenderGroup(RENDER_NONALPHA, this);

    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CWorldHeartMap::LateUpdate_GameObject(const float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CWorldHeartMap::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CWorldHeartMap::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CMapTex*>(Engine::Clone_Proto(L"Proto_WorldHeartMapTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_WorldHeartMap"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture",pComponent });


    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });


    return S_OK;
}

HRESULT CWorldHeartMap::SetUp_Material()
{
    return S_OK;
}

CWorldHeartMap* CWorldHeartMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CWorldHeartMap* pWorldHeartMap = new CWorldHeartMap(pGraphicDev);

    if (FAILED(pWorldHeartMap->Ready_GameObject()))
    {
        Safe_Release(pWorldHeartMap);
        MSG_BOX("pMap Create Failed");
        return nullptr;
    }

    return pWorldHeartMap;
}

void CWorldHeartMap::Free()
{
    Engine::CGameObject::Free();
}
