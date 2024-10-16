#include "pch.h"
#include "MoonTempleMap.h"
#include "Export_Utility.h"

CMoonTempleMap::CMoonTempleMap(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
{
}

CMoonTempleMap::~CMoonTempleMap()
{
}

HRESULT CMoonTempleMap::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
   

     
    //m_pGraphicDev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));  // 어두운 주변광 설정

    return S_OK;
}

_int CMoonTempleMap::Update_GameObject(const float& fTimeDelta)
{
    Add_RenderGroup(RENDER_NONALPHA, this);

    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CMoonTempleMap::LateUpdate_GameObject(const float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMoonTempleMap::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    //테스트
    FAILED_CHECK_RETURN(SetUp_Material(), );        

    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, false);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMoonTempleMap::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CMapTex*>(Engine::Clone_Proto(L"Proto_MoonTempleMapTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MoonTempleMap"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture",pComponent });


    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform",pComponent });

    return S_OK;
}

HRESULT CMoonTempleMap::SetUp_Material()
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

CMoonTempleMap* CMoonTempleMap::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMoonTempleMap* pMoonTempleMap = new CMoonTempleMap(pGraphicDev);

    if (FAILED(pMoonTempleMap->Ready_GameObject()))
    {
        Safe_Release(pMoonTempleMap);
        MSG_BOX("pMap Create Failed");
        return nullptr;
    }

    return pMoonTempleMap;
}

void CMoonTempleMap::Free()
{
    Engine::CGameObject::Free();
}
