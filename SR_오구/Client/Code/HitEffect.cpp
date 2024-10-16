#include "pch.h"
#include "HitEffect.h"
#include "Export_Utility.h"
#include "ResMgr.h" 

CHitEffect::CHitEffect(LPDIRECT3DDEVICE9 pGraphicDev)
    :Engine::CGameObject(pGraphicDev)
    , name(L"")
    , m_bEffect(false)
    , m_iCount(0)
{

}

CHitEffect::~CHitEffect()
{
}

HRESULT CHitEffect::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    IDirect3DTexture9* pEffectTexture1 = CResMgr::GetInstance()->GetEffectTexture()[5]; 


    m_pAnimatorCom->CreateAnimation(L"Hit_Effect", pEffectTexture1, _vec2(0.f, 0.f), _vec2(256.f, 256.f), _vec2(256.f, 0.f), 0.1f, 7);

    //m_pAnimatorCom->Play(L"Lighting_Y",false);

    //m_pTransformCom->Set_Pos(500.f, 120.f, 500.f);
    //m_pTransformCom->m_vScale = { 35.f, 100.f, 20.f }; 

    m_pTransformCom->m_vScale = { 100.f,100.f,100.f };
    m_pAnimatorCom->Play(L"Hit_Effect", false); 

    return S_OK;
}

_int CHitEffect::Update_GameObject(const _float& fTimeDelta)
{
    Add_RenderGroup(RENDER_ALPHA, this);    
    return Engine::CGameObject::Update_GameObject(fTimeDelta);  
}

void CHitEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta); 
}

void CHitEffect::Render_GameObject()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());   
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);    

    m_pGraphicDev->SetTexture(0, CResMgr::GetInstance()->GetEffectTexture()[5]);    
    m_pAnimatorCom->render();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CHitEffect::Add_Component()
{
    CComponent* pComponent = NULL;

    pComponent = m_pAnimatorCom = dynamic_cast<CAnimator2*>(Engine::Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    return S_OK;
}

CHitEffect* CHitEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CHitEffect* pHitEffect = new CHitEffect(pGraphicDev);

    if (FAILED(pHitEffect->Ready_GameObject())) 
    {
        Safe_Release(pHitEffect);   
        MSG_BOX("pObject Create Failed");   
        return nullptr;
    }

    return pHitEffect;  
}

void CHitEffect::Free()
{
    Engine::CGameObject::Free();    
}

void CHitEffect::OnCollision(CGameObject* _pOther)
{
    int a = 4;
}
