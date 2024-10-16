#include "pch.h"
#include "MoleSide.h"

CMoleSide::CMoleSide(LPDIRECT3DDEVICE9 pGraphicDev)
    :CAudience(pGraphicDev)
{
}

CMoleSide::~CMoleSide()
{
}

HRESULT CMoleSide::Ready_GameObject()
{
    SetObjectType(OBJ_TYPE::TALK_ABLE);
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/Audience.png", &m_pTex);
    m_pAnimatorCom->CreateAnimation(L"MoleRight", m_pTex, _vec2(0.f, 0.f), _vec2(128.f, 128.f), _vec2(128.f, 0.f), 0.1f, 5);
    m_pAnimatorCom->CreateAnimation(L"MoleLeft", m_pTex, _vec2(0.f, 128.f), _vec2(128.f, 128.f), _vec2(128.f, 0.f), 0.1f, 5);
    m_pAnimatorCom->Play(L"MoleLeft", true);

    return S_OK;

}

void CMoleSide::LateReady_GameObject()
{
}

_int CMoleSide::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CMoleSide::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMoleSide::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetTexture(0, m_pTex);
    m_pAnimatorCom->render();
}

HRESULT CMoleSide::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 15.f, 15.f, 15.f };
    m_pTransformCom->Set_Pos(800.f, 20.f, 580.f);

    return S_OK;

}

CMoleSide* CMoleSide::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMoleSide* pNPC = new CMoleSide(pGraphicDev);

    if (FAILED(pNPC->Ready_GameObject()))
    {
        Safe_Release(pNPC);
        MSG_BOX("CMoleSide Create Failed");
        return nullptr;
    }

    return pNPC;
}

void CMoleSide::Free()
{
    Engine::CGameObject::Free();
}
