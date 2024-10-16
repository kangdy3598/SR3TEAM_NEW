#include "pch.h"
#include "CGodEffect.h"

CGodEffect::CGodEffect(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev), m_bCall(false)
{
}

CGodEffect::~CGodEffect()
{
}

HRESULT CGodEffect::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    D3DXCreateTextureFromFile(m_pGraphicDev, L"../Bin/Resource/Texture/Effect/GodEffect.png", &m_pTex);
    m_pAnimatorCom->CreateAnimation(L"GodEffect", m_pTex, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.12f, 3);

    return S_OK;
}

void CGodEffect::LateReady_GameObject()
{
}

_int CGodEffect::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_bCall)
        return 0;
    Add_RenderGroup(RENDER_TRANSLUCNET, this);
    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CGodEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (!m_bCall)
        return;

    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CGodEffect::Render_GameObject()
{
    if (!m_bCall)
        return;
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    m_pGraphicDev->SetTexture(0, m_pTex);
    m_pTransformCom->m_vInfo[INFO_POS].y += 0.5f;
    m_pAnimatorCom->Play(L"GodEffect", false);
    m_pAnimatorCom->render();

    if (m_pAnimatorCom->GetAnimation()->IsFinish())
    {
        m_pAnimatorCom->GetAnimation()->SetFrame(0);
        m_bCall = false;
        m_pGod = dynamic_cast<CGod*>(Engine::Get_GameObject(L"Layer_GameLogic", L"God"));
        NULL_CHECK_RETURN(m_pGod);
        m_pGod->Call_God();
        Engine::Play_Sound(L"SFX_830_Blink.wav", SOUND_MOREEFFECT, 1.f);

    }

}

HRESULT CGodEffect::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_AniBuffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });
    m_pTransformCom->m_vScale = { 50.f, 50.f, 50.f };
    m_pTransformCom->Set_Pos(1170.f, 50.f, 1050.f);

    return S_OK;
}

CGodEffect* CGodEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CGodEffect* pNPC = new CGodEffect(pGraphicDev);

    if (FAILED(pNPC->Ready_GameObject()))
    {
        Safe_Release(pNPC);
        MSG_BOX("CGodEffect Create Failed");
        return nullptr;
    }

    return pNPC;
}

void CGodEffect::Free()
{
    Engine::CGameObject::Free();
}
