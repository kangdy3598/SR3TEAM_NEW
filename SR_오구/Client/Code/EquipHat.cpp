#include "pch.h"
#include "EquipHat.h"
#include "Player.h" //0924

CEquipHat::CEquipHat(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , m_pTextureCom(nullptr)
{
}

CEquipHat::~CEquipHat()
{
}

HRESULT CEquipHat::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pTransformCom->m_vScale = { 15.f,15.f,15.f };

    return S_OK;
}

void CEquipHat::LateReady_GameObject()
{
    m_pPlayer = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Player"));
    NULL_CHECK_RETURN(m_pPlayer);

    m_pPlayer->SetEquipHat(this);
}

_int CEquipHat::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    _vec3 playerPos;
    dynamic_cast<CTransform*>(
        m_pPlayer->Get_Component(ID_DYNAMIC, L"Com_Transform")
        )->Get_Info(INFO_POS, &playerPos);

    playerPos.y += 10;
    playerPos.z -= 2;
    m_pTransformCom->Set_Pos(playerPos);

    Add_RenderGroup(RENDER_ALPHA, this);

    
    return iExit;
}

void CEquipHat::LateUpdate_GameObject(const _float& fTimeDelta)
{
    
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CEquipHat::Render_GameObject()
{
    if (!m_pTextureCom)
        return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture();
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); 
    m_pGraphicDev->SetTexture(0, NULL);
}

HRESULT CEquipHat::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    return S_OK;
}

CEquipHat* CEquipHat::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CEquipHat* pEquipHat = new CEquipHat(pGraphicDev);

    if (FAILED(pEquipHat->Ready_GameObject()))
    {
        Safe_Release(pEquipHat);
        MSG_BOX("EquipHat Create Failed");
        return nullptr;
    }

    return pEquipHat;
}

void CEquipHat::Free()
{
    Engine::CGameObject::Free();
}
