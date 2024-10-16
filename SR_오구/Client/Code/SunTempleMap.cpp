#include "pch.h"
#include "SunTempleMap.h"
#include "Export_Utility.h"

CSunTempleMap::CSunTempleMap(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
{
}

CSunTempleMap::~CSunTempleMap()
{
}

HRESULT CSunTempleMap::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    return S_OK;
}

_int CSunTempleMap::Update_GameObject(const float& fTimeDelta)
{
    Add_RenderGroup(RENDER_NONALPHA, this);

    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CSunTempleMap::LateUpdate_GameObject(const float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CSunTempleMap::Render_GameObject()
{
    
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

    FAILED_CHECK_RETURN(SetUp_Material(), );
    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, false);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CSunTempleMap::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CMapTex*>(Engine::Clone_Proto(L"Proto_SunTempleMapTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_SunTempleMap"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture",pComponent });


    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });

    return S_OK;
}

HRESULT CSunTempleMap::SetUp_Material()
{
    D3DMATERIAL9		tMtrl;  
    ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));   

    tMtrl.Diffuse = { 0.5f, 0.5f, 0.5f, 0.5f }; 
    tMtrl.Specular = { 0.5f, 0.5f, 0.5f, 0.5f };    
    tMtrl.Ambient = { 0.3f, 0.3f, 0.3f, 0.3f }; 

    tMtrl.Emissive = { 0.3f,0.3f, 0.3f, 0.3f }; 
    tMtrl.Power = 0.f;  

    m_pGraphicDev->SetMaterial(&tMtrl); 

    return S_OK;
}

CSunTempleMap* CSunTempleMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CSunTempleMap* pSunTempleMap = new CSunTempleMap(pGraphicDev);  

    if (FAILED(pSunTempleMap->Ready_GameObject()))  
    {
        Safe_Release(pSunTempleMap);    
        MSG_BOX("pMap Create Failed");      
        return nullptr; 
    }   

    return pSunTempleMap;   
}

void CSunTempleMap::Free()
{
    Engine::CGameObject::Free();        
}
