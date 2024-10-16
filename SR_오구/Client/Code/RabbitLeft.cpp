#include "pch.h"
#include "RabbitLeft.h"

CRabbitLeft::CRabbitLeft(LPDIRECT3DDEVICE9 pGraphicDev)
    :CAudience(pGraphicDev)
{
}

CRabbitLeft::~CRabbitLeft()
{
}

HRESULT CRabbitLeft::Ready_GameObject()
{
    SetObjectType(OBJ_TYPE::TALK_ABLE);
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/NPC/CheerRabbitSide.png", &m_pTex);
    m_pAnimatorCom->CreateAnimation(L"CheerRabbitLeft", m_pTex, _vec2(0.f, 0.f), _vec2(128.f, 128.f), _vec2(128.f, 0.f), 0.1f, 3);
    m_pAnimatorCom->CreateAnimation(L"CheerRabbitRight", m_pTex, _vec2(0.f, 128.f), _vec2(128.f, 128.f), _vec2(128.f, 0.f), 0.1f, 3);
    m_pAnimatorCom->Play(L"CheerRabbitLeft", true);

    return S_OK;
}

void CRabbitLeft::LateReady_GameObject()
{
}

_int CRabbitLeft::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CRabbitLeft::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CRabbitLeft::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetTexture(0, m_pTex);
    m_pAnimatorCom->render();
}

HRESULT CRabbitLeft::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Ani_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 20.f, 20.f, 20.f };
    m_pTransformCom->Set_Pos(800.f, 20.f, 600.f);

    return S_OK;
}

CRabbitLeft* CRabbitLeft::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CRabbitLeft* pNPC = new CRabbitLeft(pGraphicDev);

    if (FAILED(pNPC->Ready_GameObject()))
    {
        Safe_Release(pNPC);
        MSG_BOX("CRabbitSide Create Failed");
        return nullptr;
    }

    return pNPC;

}

void CRabbitLeft::Free()
{
    Engine::CGameObject::Free();
}
