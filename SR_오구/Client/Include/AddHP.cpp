#include "pch.h"
#include "AddHP.h"

CAddHP::CAddHP(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev), m_bCall(false)
{
}

CAddHP::~CAddHP()
{
}

HRESULT CAddHP::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/UI/AddHP.png", &m_pTex);
    m_pAnimatorCom->CreateAnimation(L"AddHP", m_pTex, _vec2(0.f, 0.f), _vec2(512.f, 128.f), _vec2(512.f, 0.f), 0.12f, 3);

    return S_OK;
}

void CAddHP::LateReady_GameObject()
{
}

_int CAddHP::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_bCall)
        return 0;
    Add_RenderGroup(RENDER_UI, this);
    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CAddHP::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (!m_bCall)
        return;

    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CAddHP::Render_GameObject()
{
    if (!m_bCall)
        return;
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetTexture(0, m_pTex);
    m_pAnimatorCom->Play(L"AddHP", false);
    m_pAnimatorCom->render();

    if (m_pAnimatorCom->GetAnimation()->IsFinish())
    {
        m_pAnimatorCom->GetAnimation()->SetFrame(0);
        m_bCall = false;
    }
}

HRESULT CAddHP::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_AniBuffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 180.f, 50.f, 1.f };
    m_pTransformCom->m_vInfo[INFO_POS] = { -420.f, 310.f, 0.1f };

    return S_OK;

}

CAddHP* CAddHP::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CAddHP* pEffect = new CAddHP(pGraphicDev);

    if (FAILED(pEffect->Ready_GameObject()))
    {
        Safe_Release(pEffect);
        MSG_BOX("CAddHP Create Failed");
        return nullptr;
    }

    return pEffect;
}

void CAddHP::Free()
{
    Engine::CGameObject::Free();
}
